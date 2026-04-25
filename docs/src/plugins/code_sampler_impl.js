const fs = require('fs')
const path = require('path')

const default_extensions = [
    '.c',
    '.cc',
    '.cxx',
    '.cxxm',
    '.cpp',
    '.cppm',
    '.c++',
    '.c++m',
    '.ii',
    '.ixx',
    '.ipp',
    '.i++',
    '.inl',
    '.h',
    '.hh',
    '.hxx',
    '.hpp',
    '.h++',
    '.ixx',
    '.inc',
    '.m',
    '.mm',
]

function has_source_extension(p, options) {
    return options.extensions.includes(path.extname(p))
}

const default_roots = [
    '../src',
    '../include'
]

function visit_all_files(dir, options) {
    let retval = []

    const paths = fs.readdirSync(dir)

    for (const p_ of paths) {
        const p = path.join(dir, p_)
        const stat = fs.statSync(p)

        if (stat.isDirectory())
            retval = retval.concat(visit_all_files(p, options))
        else if (has_source_extension(p, options))
            retval.push(p)
    }

    return retval
}

function get_source_files(options) {
    let retval = []
    for (const root of options.roots) {
        retval = retval.concat(visit_all_files(root, options))
    }
    var i = 0
    retval = retval.map(
        (f) => {
            i = 0
            // Note: The 1-based numbering is intentional.
            const numbered_lines = fs.readFileSync(f, 'utf8').split('\n').map(
                (line) => ({num: ++i, text: line}))
            return { name: f, lines: numbered_lines }
        }
    )
    return retval
}

// TODO: Change the regex to [|] followed by the identifier, where the
// identifier is optional in the ] case, but if present, is used to check that
// it matches the innermost open tag.
const comment_line_regex = /^\s*\/\/\s*(?:(?:(\[)\s*(\w*))|(\]))\s*$/

function get_tagged_regions(source_files) {
    let retval = {}
    let tag_seen_in = {}
    for (let source_file of source_files) {
        let this_file_result = {}
        let open_tags = []
        for (let line of source_file.lines) {
            const match = line.text.match(comment_line_regex)
            if (match) {
                if (typeof match[1] === "string" &&
                    typeof match[2] === "string" &&
                    typeof match[3] === "undefined") { // begin-tag
                    open_tags.push({tag: match[2], lines: []})
                    if (!(match[2] in this_file_result))
                        this_file_result[match[2]] = []
                } else { // end-tag
                    if (open_tags.length === 0) {
                        throw new Error(`error: ${source_file.name}:${line.num}: Unexpected '// ]' close-tag comment that does not match any previous open-tag comment.`);
                    }
                    let last_open_tag = open_tags.pop()
                    if (last_open_tag.lines.length !== 0) {
                        this_file_result[last_open_tag.tag] =
                            this_file_result[last_open_tag.tag].concat(
                                last_open_tag.lines)
                    }
                }
            } else {
                open_tags = open_tags.map(
                    (t) => ({tag: t.tag, lines: t.lines.concat([line])}))
            }
        }
        if (open_tags.length !== 0) {
            throw new Error(`error: ${source_file.name}: Tag(s) ${open_tags.map((elem) => elem.tag).map((elem) => "'" + elem + "'").join(", ")} left open at end of file.`);
        }
        for (const tag in this_file_result) {
            if (tag in retval) {
                throw new Error(`error: Source file tag '${tag}' repeated in ${source_file.name}.  '${tag}' was first used in ${tag_seen_in[tag]}.`)
            } else {
                retval[tag] = []
                tag_seen_in[tag] = source_file.name
            }
            retval[tag] = retval[tag].concat(this_file_result[tag])
        }
    }
    return retval
}

module.exports = {
    default_extensions,
    default_roots,
    get_source_files,
    get_tagged_regions
}

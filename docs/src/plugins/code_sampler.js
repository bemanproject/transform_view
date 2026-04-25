import {visit} from 'unist-util-visit'
const impl = require('./code_sampler_impl.js')

const plugin = (options = {}) => {
    if (options.roots === undefined)
        options.roots = impl.default_roots;
    if (options.extensions === undefined)
        options.extensions = impl.default_extensions;
    if (options.line_numbers === undefined)
        options.line_numbers = false;
    if (options.source_line_numbers === undefined)
        options.source_line_numbers = false;

    const transformer = async (ast, vfile) => {
        let regions = null
        visit(ast, 'code', (node) => {
            if (regions === null) {
                regions = impl.get_tagged_regions(
                    impl.get_source_files(options))
            }

            if ((node.lang === 'cpp' || node.lang === 'c++') &&
                node.meta !== null &&
                node.value === '') {
                if (!(node.meta in regions)) {
                    throw Error(`error: ${vfile.path}:${node.position.start.line}: Unknown source tag '${node.meta}'.`)
                }
                node.value = regions[node.meta].map((e) => e.text).join("\n")
            }
        });
    };

    return transformer;
};

export default plugin;

#!/usr/bin/env python3

import argparse
import getpass
import os
import sys
try:
    from git import Repo
except:
    print("You do not have GitPython installed, but it is required.  Ubuntu users can install with 'sudo apt-get install python3-git'.  Other users, see https://gitpython.readthedocs.io/en/stable/intro.html.")
    sys.exit(1)

parser = argparse.ArgumentParser(description="Adapts the Beman exemplar project for your use as a brand new project.  Leaves 'TODO' in several places that need you attention (especially in README.md).")
parser.add_argument('project_name', type=str, help="The name of your new project.  This should be the same name as the Github repo name.  For example, 'my_proj' in 'git@github.com:me/my_proj.git'")
parser.add_argument('--owner', type=str, default=getpass.getuser(), help="The user or group in which this project's Github repo is found.  For example, 'me' in 'git@github.com:me/my_proj.git'.  Defaults to your username on this system.")
parser.add_argument('--paper', type=str, default="TODO", help="The paper that this Beman project is implementing (e.g. P1234R5).")
parser.add_argument('--cpp-version', type=int, default=26, help="The C++ version required to build this project.")
parser.add_argument('--desc', type=str, default="TODO", help="The description of this project that should appear in the title of your README.md.")
args = parser.parse_args()

subtrees = ['src/beman', 'include/beman', 'tests/beman']

# Before making any concrete changes, move 'exemplar' subtrees via 'git mv'.
repo = Repo('.')
for tree in subtrees:
    repo.index.move([f'{tree}/exemplar', f'{tree}/{args.project_name}'])

# Replace old README.md with new one.
readme = f'''<!--
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
-->

# beman.{args.project_name}: {args.desc}
![Continuous Integration Tests](https://github.com/{args.owner}/{args.project_name}/actions/workflows/ci_tests.yml/badge.svg)


<!-- markdownlint-disable -->
<img src="https://github.com/bemanproject/beman/blob/main/images/logos/beman_logo-beman_library_under_development.png" style="width:5%; height:auto;"> ![Continuous Integration Tests](https://github.com/{args.owner}/{args.project_name}/actions/workflows/ci_tests.yml/badge.svg)
![Code Format](https://github.com/bemanproject/{args.owner}/{args.project_name}/actions/workflows/pre-commit.yml/badge.svg)
<!-- markdownlint-enable -->

**Implements**: [`{args.project_name}` ({args.paper})](https://wg21.link/{args.paper})

<!-- markdownlint-disable -->
**Status**: [Under development and not yet ready for production use.](https://github.com/bemanproject/beman/blob/main/docs/BEMAN_LIBRARY_MATURITY_MODEL.md#under-development-and-not-yet-ready-for-production-use)
<!-- markdownlint-enable -->

## Usage

TODO

## How to Build

TODO

### Compiler support

TODO

Building this repository requires **C++{args.cpp_version}** or later.

### Dependencies

TODO

'''
with open('README.md', 'w') as f: f.write(readme)

# Change project name in top-level CMakeLists.txt.
cmakelists_lines = None
with open('CMakeLists.txt') as f: cmakelists_lines = f.readlines()
file_contents = ''
for line in cmakelists_lines:
    if 'DESCRIPTION' in line:
        indent = line.find('DESCRIPTION')
        file_contents += (' ' * indent) + f'DESCRIPTION "{args.desc}"' + '\n'
    else:
        file_contents += line.replace('exemplar', args.project_name)
with open('CMakeLists.txt', 'w') as f: f.write(file_contents)

# Change 'exemplar' to args.project_name in all files under subtrees[].
for tree in subtrees + ['examples']:
    for root, dirs, files in os.walk(tree):
        for f in files:
            path = os.path.join(root, f)
            contents = ''
            with open(path) as f:
                contents = f.read()
            with open(path, 'w') as f:
                f.write(contents.replace('exemplar', args.project_name))

# 'git add' all changed files.
repo.index.add([item.a_path for item in repo.index.diff(None)])

# Remove this file; it's only need once.
repo.index.remove('new_project_from_exemplar.py')

# Commit the changes.
repo.index.commit("Ran new_project_from_exemplar.py on project.")

print(f'''Success!
All references to 'exemplar' have been replaced with '{args.project_name}'
throughout the project, and new_project_from_exemplar.py has been deleted.
The results of these changes have been committed.  Please go replace all
instances of 'TODO' with the approporate text, especially in README.md.
''')

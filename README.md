# beman.transform_view: A conditionally borrowed `std::ranges::transform_view`

<!--
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
-->

![Library Status](https://raw.githubusercontent.com/bemanproject/beman/refs/heads/main/images/badges/beman_badge-beman_library_under_development.svg) ![Continuous Integration Tests](https://github.com/bemanproject/transform_view/actions/workflows/ci_tests.yml/badge.svg) ![Lint Check (pre-commit)](https://github.com/bemanproject/transform_view/actions/workflows/pre-commit.yml/badge.svg) [![Coverage](https://coveralls.io/repos/github/bemanproject/transform_view/badge.svg?branch=main)](https://coveralls.io/github/bemanproject/transform_view?branch=main) ![Standard Target](https://github.com/bemanproject/beman/blob/main/images/badges/cpp29.svg)

**Implements**: [`transform_view` (P3117)](https://wg21.link/P3117)

**Status**: [Under development and not yet ready for production use.](https://github.com/bemanproject/beman/blob/main/docs/beman_library_maturity_model.md#under-development-and-not-yet-ready-for-production-use)

This library contains only `beman::transform_view::transform_view` and its
associated view adaptor `beman::transform_view::transform`.  These work
exactly like `std::ranges::transform_view` and `std::ranges::transform`,
except that the `beman` `transform_view` is a `borrowable_range` if its
adapted view `V` is borrowable, and if `detail::tidy_func<F>` is true for the
callable `F`.  `detail::tidy_func` is defined like this:

```c++
template <class F>
constexpr bool tidy_func =
    std::is_empty_v<F> && std::is_trivially_default_constructible_v<F> &&
    std::is_trivially_destructible_v<F>;
```

If a `transform_view` is borrowable, its `iterator` re-creates `F` each time
it uses `F`, rather than going back to the parent `transform_view`.

## License

`beman.transform_view` is licensed under the Apache License v2.0 with LLVM Exceptions.

### Usage

```c++
#include <beman/transform_view/transform_view.hpp>

#include <iostream>

namespace tv26 = beman::transform_view;

int main() {
    auto to_lower = [](char c) { return char(c + 0x20); };

    const std::string upper_str = "LOWER";

    std::cout << (upper_str | tv26::views::transform(to_lower) |
                  std::ranges::to<std::string>())
              << '\n';

    return 0;
}
```

See online documentation at https://tzlaine.github.io/transform_view .

## Dependencies

### Build Environment

This project requires at least the following to build:

* A C++ compiler that conforms to the C++23 standard or greater
* CMake 3.28 or later
* (Test Only) GoogleTest

You can disable building tests by setting CMake option
[`BEMAN_TRANSFORM_VIEW_BUILD_TESTS`](#beman_transform_view_build_tests) to `OFF`
when configuring the project.

### Supported Platforms

| Compiler   | Version | C++ Standards | Standard Library  |
|------------|---------|---------------|-------------------|
| GCC        | 15-14   | C++26, C++23  | libstdc++         |
| Clang      | 22-19   | C++26, C++23  | libstdc++, libc++ |
| Clang      | 18      | C++23         | libstdc++         |
| AppleClang | latest  | C++26, C++23  | libc++            |
| MSVC       | latest  | C++23         | MSVC STL          |

## Development

### Develop using GitHub Codespace

This project supports [GitHub Codespace](https://github.com/features/codespaces)
via [Development Containers](https://containers.dev/),
which allows rapid development and instant hacking in your browser.
We recommend using GitHub codespace to explore this project as it
requires minimal setup.

Click the following badge to create a codespace:

[![Open in GitHub Codespaces](https://github.com/codespaces/badge.svg)](https://codespaces.new/bemanproject/transform_view)

For more documentation on GitHub codespaces, please see
[this doc](https://docs.github.com/en/codespaces/).

> [!NOTE]
>
> The codespace container may take up to 5 minutes to build and spin-up; this is normal.

### Develop locally on your machines

<details>
<summary> For Linux </summary>

Beman libraries require [recent versions of CMake](#build-environment),
we recommend downloading CMake directly from [CMake's website](https://cmake.org/download/)
or installing it with the [Kitware apt library](https://apt.kitware.com/).

A [supported compiler](#supported-platforms) should be available from your package manager.

</details>

<details>
<summary> For MacOS </summary>

Beman libraries require [recent versions of CMake](#build-environment).
Use [`Homebrew`](https://brew.sh/) to install the latest version of CMake.

```bash
brew install cmake
```

A [supported compiler](#supported-platforms) is also available from brew.

For example, you can install the latest major release of Clang as:

```bash
brew install llvm
```

</details>

<details>
<summary> For Windows </summary>

To build Beman libraries, you will need the MSVC compiler. MSVC can be obtained
by installing Visual Studio; the free Visual Studio 2022 Community Edition can
be downloaded from
[Microsoft](https://visualstudio.microsoft.com/vs/community/).

After Visual Studio has been installed, you can launch "Developer PowerShell for
VS 2022" by typing it into Windows search bar. This shell environment will
provide CMake, Ninja, and MSVC, allowing you to build the library and run the
tests.

Note that you will need to use FetchContent to build GoogleTest. To do so,
please see the instructions in the "Build GoogleTest dependency from github.com"
dropdown in the [Project specific configure
arguments](#project-specific-configure-arguments) section.

</details>

### Configure and Build the Project Using CMake Presets

This project recommends using [CMake Presets](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html)
to configure, build and test the project.
Appropriate presets for major compilers have been included by default.
You can use `cmake --list-presets` to see all available presets.

Here is an example to invoke the `gcc-debug` preset.

```shell
cmake --workflow --preset gcc-debug
```

Generally, there are two kinds of presets, `debug` and `release`.

The `debug` presets are designed to aid development, so it has debugging
instrumentation enabled and many sanitizers enabled.

> [!NOTE]
>
> The sanitizers that are enabled vary from compiler to compiler.
> See the toolchain files under ([`cmake`](cmake/)) to determine the exact configuration used for each preset.

The `release` presets are designed for production use, and
consequently have the highest optimization turned on (e.g. `O3`).

### Configure and Build Manually

If the presets are not suitable for your use-case, a traditional CMake
invocation will provide more configurability.

To configure, build and test the project with extra arguments,
you can run this set of commands.

```bash
cmake \
  -B build \
  -S . \
  -DCMAKE_CXX_STANDARD=20 \
  -DCMAKE_PREFIX_PATH=$PWD/infra/cmake \
  # Your extra arguments here.
cmake --build build
ctest --test-dir build
```

> [!IMPORTANT]
>
> Beman projects are
> [passive projects](https://github.com/bemanproject/beman/blob/main/docs/beman_standard.md#cmake),
> therefore,
> you will need to specify the C++ version via `CMAKE_CXX_STANDARD`
> when manually configuring the project.

### Finding and Fetching GTest from GitHub

If you do not have GoogleTest installed on your development system, you may
optionally configure this project to download a known-compatible release of
GoogleTest from source and build it as well.

Example commands:

```shell
cmake -B build -S . \
    -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES=./infra/cmake/use-fetch-content.cmake \
    -DCMAKE_CXX_STANDARD=20
cmake --build build --target all
cmake --build build --target test
```

The precise version of GoogleTest that will be used is maintained in
`./lockfile.json`.

### Project specific configure arguments

Project-specific options are prefixed with `BEMAN_TRANSFORM_VIEW`.
You can see the list of available options with:

```bash
cmake -LH -S . -B build | grep "BEMAN_TRANSFORM_VIEW" -C 2
```

<details>

<summary> Details of CMake arguments. </summary>

#### `BEMAN_TRANSFORM_VIEW_BUILD_TESTS`

Enable building tests and test infrastructure. Default: ON.
Values: `{ ON, OFF }`.

You can configure the project to have this option turned off via:

```bash
cmake -B build -S . -DCMAKE_CXX_STANDARD=20 -DBEMAN_TRANSFORM_VIEW_BUILD_TESTS=OFF
```

> [!TIP]
> Because this project requires GoogleTest for running tests,
> disabling `BEMAN_TRANSFORM_VIEW_BUILD_TESTS` avoids the project from
> cloning GoogleTest from GitHub.

#### `BEMAN_TRANSFORM_VIEW_BUILD_EXAMPLES`

Enable building examples. Default: ON. Values: { ON, OFF }.

#### `BEMAN_TRANSFORM_VIEW_INSTALL_CONFIG_FILE_PACKAGE`

Enable installing the CMake config file package. Default: ON.
Values: { ON, OFF }.

This is required so that users of `beman.transform_view` can use
`find_package(beman.transform_view)` to locate the library.

</details>

## Integrate beman.transform_view into your project

To use `beman.transform_view` in your C++ project,
include an appropriate `beman.transform_view` header from your source code.

```c++
#include <beman/transform_view/transform_view.hpp>
```

> [!NOTE]
>
> `beman.transform_view` headers are to be included with the `beman/transform_view/` prefix.
> Altering include search paths to spell the include target another way (e.g.
> `#include <transform_view.hpp>`) is unsupported.

The process for incorporating `beman.transform_view` into your project depends on the
build system being used. Instructions for CMake are provided in following sections.

### Incorporating `beman.transform_view` into your project with CMake

For CMake based projects,
you will need to use the `beman.transform_view` CMake module
to define the `beman::transform_view` CMake target:

```cmake
find_package(beman.transform_view REQUIRED)
```

You will also need to add `beman::transform_view` to the link libraries of
any libraries or executables that include `beman.transform_view` headers.

```cmake
target_link_libraries(yourlib PUBLIC beman::transform_view)
```

### Produce beman.transform_view interface library

You can produce transform_view's interface library locally by:

```bash
cmake --workflow --preset gcc-release
cmake --install build/gcc-release --prefix /opt/beman
```

This will generate the following directory structure at `/opt/beman`.

```txt
/opt/beman
├── include
│   └── beman
│       └── transform_view
│           └── transform_view.hpp
└── lib
    └── cmake
        └── beman.transform_view
            ├── beman.transform_view-config-version.cmake
            ├── beman.transform_view-config.cmake
            └── beman.transform_view-targets.cmake
```

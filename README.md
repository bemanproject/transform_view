# beman.transform_view: A conditionally borrowed `std::ranges::transform_view`

<!--
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
-->

<!-- markdownlint-disable-next-line line-length -->
![Library Status](https://raw.githubusercontent.com/bemanproject/beman/refs/heads/main/images/badges/beman_badge-beman_library_under_development.svg) ![Continuous Integration Tests](https://github.com/bemanproject/transform_view/actions/workflows/ci_tests.yml/badge.svg) ![Lint Check (pre-commit)](https://github.com/bemanproject/transform_view/actions/workflows/pre-commit-check.yml/badge.svg) [![Coverage](https://coveralls.io/repos/github/bemanproject/transform_view/badge.svg?branch=main)](https://coveralls.io/github/bemanproject/transform_view?branch=main) ![Standard Target](https://github.com/bemanproject/beman/blob/main/images/badges/cpp29.svg)

**Implements**: [`transform_view` (P3117R0)](https://wg21.link/P3117R0)

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

## Usage

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

Full runnable examples can be found in [`examples/`](examples/).

## Dependencies

### Build Environment

This project requires at least the following to build:

* A C++ compiler that conforms to the C++23 standard or greater
* CMake 3.30 or later
* (Test Only) GoogleTest

You can disable building tests by setting CMake option `BEMAN_TRANSFORM_VIEW_BUILD_TESTS` to
`OFF` when configuring the project.

### Supported Platforms

| Compiler   | Version | C++ Standards | Standard Library  |
|------------|---------|---------------|-------------------|
| GCC        | 15-14   | C++26, C++23  | libstdc++         |
| Clang      | 22-19   | C++26, C++23  | libstdc++, libc++ |
| Clang      | 18      | C++23         | libstdc++         |
| AppleClang | latest  | C++26, C++23  | libc++            |
| MSVC       | latest  | C++23         | MSVC STL          |

## Development

See the [Contributing Guidelines](CONTRIBUTING.md).

## Integrate beman.transform_view into your project

### Build

You can build transform_view using a CMake workflow preset:

```bash
cmake --workflow --preset gcc-release
```

To list available workflow presets, you can invoke:

```bash
cmake --list-presets=workflow
```

For details on building beman.transform_view without using a CMake preset, refer to the
[Contributing Guidelines](CONTRIBUTING.md).

### Installation

To install beman.transform_view globally after building with the `gcc-release` preset, you can
run:

```bash
sudo cmake --install build/gcc-release
```

Alternatively, to install to a prefix, for example `/opt/beman`, you can run:

```bash
sudo cmake --install build/gcc-release --prefix /opt/beman
```

This will generate the following directory structure:

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

### CMake Configuration

If you installed beman.transform_view to a prefix, you can specify that prefix to your CMake
project using `CMAKE_PREFIX_PATH`; for example, `-DCMAKE_PREFIX_PATH=/opt/beman`.

You need to bring in the `beman.transform_view` package to define the `beman::transform_view` CMake
target:

```cmake
find_package(beman.transform_view REQUIRED)
```

You will then need to add `beman::transform_view` to the link libraries of any libraries or
executables that include `beman.transform_view` headers.

```cmake
target_link_libraries(yourlib PUBLIC beman::transform_view)
```

### Using beman.transform_view

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
cmake -B build -S . -DCMAKE_CXX_STANDARD=23 -DBEMAN_TRANSFORM_VIEW_BUILD_TESTS=OFF
```

#### `BEMAN_TRANSFORM_VIEW_BUILD_EXAMPLES`

Enable building examples. Default: ON. Values: { ON, OFF }.

#### `BEMAN_TRANSFORM_VIEW_INSTALL_CONFIG_FILE_PACKAGE`

Enable installing the CMake config file package. Default: ON.
Values: { ON, OFF }.

This is required so that users of `beman.transform_view` can use
`find_package(beman.transform_view)` to locate the library.

</details>

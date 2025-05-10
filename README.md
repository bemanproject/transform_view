<!--
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
-->

# beman.transform_view: A conditionally borrowed `std::ranges::transform_view`

<img src="https://github.com/bemanproject/beman/blob/main/images/logos/beman_logo-beman_library_under_development.png" style="width:5%; height:auto;"> ![Continuous Integration Tests](https://github.com/tzlaine/transform_view/actions/workflows/ci_tests.yml/badge.svg) ![Code Format](https://github.com/tzlaine/transform_view/actions/workflows/pre-commit.yml/badge.svg)

**Implements**: [`transform_view` (P3117)](https://wg21.link/P3117)

**Status**: [Under development and not yet ready for production use.](https://github.com/bemanproject/beman/blob/main/docs/BEMAN_LIBRARY_MATURITY_MODEL.md#under-development-and-not-yet-ready-for-production-use)

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

## Building beman.transform_view

### Dependencies
This project has no C or C++ dependencies.

Build-time dependencies:

- `cmake`
- `ninja`, `make`, or another CMake-supported build system
  - CMake defaults to "Unix Makefiles" on POSIX systems

#### How to install dependencies

<details>
<summary>Dependencies install transform_view on Ubuntu 24.04  </summary>

```shell
# Install tools:
apt-get install -y cmake make ninja-build

# Toolchains:
apt-get install                           \
  g++-14 gcc-14                           \
  clang-18 clang++-18 clang-19 clang++-19
```

</details>

<details>
<summary>Dependencies install transform_view on MAC OS $VERSION </summary>

<!-- TODO Darius: rewrite section!-->
```shell
# TODO
```

</details>

<details>
<summary>Dependencies install transform_view on Windows $VERSION  </summary>
<!-- TODO Darius: rewrite section!-->

```shell
# TODO
```

</details>

### How to build beman.transform_view

This project strives to be as normal and simple a CMake project as possible.
This build workflow in particular will work,
producing a static `libbeman.transform_view.a` library, ready to package with its headers:

```shell
cmake --workflow --preset gcc-debug
cmake --workflow --preset gcc-release
cmake --install build/gcc-release --prefix /opt/beman.transform_view
```

<details>
<summary> Build beman.transform_view (verbose logs) </summary>

```shell
# Configure beman.transform_view via gcc-debug workflow for development.
$ cmake --workflow --preset gcc-debug
Executing workflow step 1 of 3: configure preset "gcc-debug"

Preset CMake variables:

  CMAKE_BUILD_TYPE="Debug"
  CMAKE_CXX_COMPILER="g++-14"
  CMAKE_CXX_FLAGS="-fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract -fsanitize=leak -fsanitize=undefined"
  CMAKE_CXX_STANDARD="23"

Examples to be built: transform_view_direct_usage
-- Configuring done (0.3s)
-- Generating done (0.0s)
-- Build files have been written to: /home/tzlaine/transform_view/build/gcc-debug

Executing workflow step 2 of 3: build preset "gcc-debug"

[6/6] Linking CXX executable tests/beman/transform_view/beman.transform_view.tests.transform_view

Executing workflow step 3 of 3: test preset "gcc-debug"

Test project /home/tzlaine/transform_view/build/gcc-debug
      Start  1: transform_view_.iter_concept_categroy
 1/25 Test  #1: transform_view_.iter_concept_categroy ......................   Passed    0.01 sec
      Start  2: transform_view_.default_ctor
 2/25 Test  #2: transform_view_.default_ctor ...............................   Passed    0.02 sec
      Start  3: transform_view_.base_
 3/25 Test  #3: transform_view_.base_ ......................................   Passed    0.03 sec
      Start  4: transform_view_.begin_end
 4/25 Test  #4: transform_view_.begin_end ..................................   Passed    0.02 sec
      Start  5: transform_view_.copy_func_empty_range_copy_alg
 5/25 Test  #5: transform_view_.copy_func_empty_range_copy_alg .............   Passed    0.01 sec
      Start  6: transform_view_.copy_func_copy_alg
 6/25 Test  #6: transform_view_.copy_func_copy_alg .........................   Passed    0.03 sec
      Start  7: transform_view_.copy_func_empty_range_adaptor_copy_alg
 7/25 Test  #7: transform_view_.copy_func_empty_range_adaptor_copy_alg .....   Passed    0.03 sec
      Start  8: transform_view_.copy_func_adaptor_copy_alg
 8/25 Test  #8: transform_view_.copy_func_adaptor_copy_alg .................   Passed    0.02 sec
      Start  9: transform_view_.copy_func_empty_range_adaptor_to_vec
 9/25 Test  #9: transform_view_.copy_func_empty_range_adaptor_to_vec .......   Passed    0.04 sec
      Start 10: transform_view_.copy_func_adaptor_to_vec
10/25 Test #10: transform_view_.copy_func_adaptor_to_vec ...................   Passed    0.03 sec
      Start 11: transform_view_.lower_func_copy_alg
11/25 Test #11: transform_view_.lower_func_copy_alg ........................   Passed    0.02 sec
      Start 12: transform_view_.lower_func_range_adaptor
12/25 Test #12: transform_view_.lower_func_range_adaptor ...................   Passed    0.03 sec
      Start 13: transform_view_.lower_func_adaptor_to_vec
13/25 Test #13: transform_view_.lower_func_adaptor_to_vec ..................   Passed    0.04 sec
      Start 14: transform_view_.lower_upper_func_copy_alg
14/25 Test #14: transform_view_.lower_upper_func_copy_alg ..................   Passed    0.04 sec
      Start 15: transform_view_.lower_upper_func_range_adaptor
15/25 Test #15: transform_view_.lower_upper_func_range_adaptor .............   Passed    0.03 sec
      Start 16: transform_view_.lower_upper_func_adaptor_to_vec
16/25 Test #16: transform_view_.lower_upper_func_adaptor_to_vec ............   Passed    0.02 sec
      Start 17: transform_view_.sentinel_lower_func_copy_alg
17/25 Test #17: transform_view_.sentinel_lower_func_copy_alg ...............   Passed    0.02 sec
      Start 18: transform_view_.sentinel_lower_func_range_adaptor
18/25 Test #18: transform_view_.sentinel_lower_func_range_adaptor ..........   Passed    0.02 sec
      Start 19: transform_view_.sentinel_lower_func_adaptor_to_vec
19/25 Test #19: transform_view_.sentinel_lower_func_adaptor_to_vec .........   Passed    0.02 sec
      Start 20: transform_view_.sentinel_lower_upper_func_copy_alg
20/25 Test #20: transform_view_.sentinel_lower_upper_func_copy_alg .........   Passed    0.02 sec
      Start 21: transform_view_.sentinel_lower_upper_func_range_adaptor
21/25 Test #21: transform_view_.sentinel_lower_upper_func_range_adaptor ....   Passed    0.02 sec
      Start 22: transform_view_.sentinel_lower_upper_func_adaptor_to_vec
22/25 Test #22: transform_view_.sentinel_lower_upper_func_adaptor_to_vec ...   Passed    0.02 sec
      Start 23: transform_view_.borrowability
23/25 Test #23: transform_view_.borrowability ..............................   Passed    0.03 sec
      Start 24: transform_view_.borrowability_safety
24/25 Test #24: transform_view_.borrowability_safety .......................   Passed    0.03 sec
      Start 25: transform_view_.borrowability_safety_failure
25/25 Test #25: transform_view_.borrowability_safety_failure ...............   Passed    0.03 sec

100% tests passed, 0 tests failed out of 25

Total Test time (real) =   0.60 sec

# Configure beman.transform_view via gcc-release workflow for direct usage.
$ cmake --workflow --preset gcc-release
Executing workflow step 1 of 3: configure preset "gcc-release"

Preset CMake variables:

  CMAKE_BUILD_TYPE="RelWithDebInfo"
  CMAKE_CXX_COMPILER="g++-14"
  CMAKE_CXX_FLAGS="-O3"
  CMAKE_CXX_STANDARD="23"

Examples to be built: transform_view_direct_usage
-- Configuring done (0.3s)
-- Generating done (0.0s)
-- Build files have been written to: /home/tzlaine/transform_view/build/gcc-release

Executing workflow step 2 of 3: build preset "gcc-release"

[6/6] Linking CXX executable tests/beman/transform_view/beman.transform_view.tests.transform_view

Executing workflow step 3 of 3: test preset "gcc-release"

Test project /home/tzlaine/transform_view/build/gcc-release
      Start  1: transform_view_.iter_concept_categroy
 1/25 Test  #1: transform_view_.iter_concept_categroy ......................   Passed    0.00 sec
      Start  2: transform_view_.default_ctor
 2/25 Test  #2: transform_view_.default_ctor ...............................   Passed    0.00 sec
      Start  3: transform_view_.base_
 3/25 Test  #3: transform_view_.base_ ......................................   Passed    0.00 sec
      Start  4: transform_view_.begin_end
 4/25 Test  #4: transform_view_.begin_end ..................................   Passed    0.00 sec
      Start  5: transform_view_.copy_func_empty_range_copy_alg
 5/25 Test  #5: transform_view_.copy_func_empty_range_copy_alg .............   Passed    0.00 sec
      Start  6: transform_view_.copy_func_copy_alg
 6/25 Test  #6: transform_view_.copy_func_copy_alg .........................   Passed    0.00 sec
      Start  7: transform_view_.copy_func_empty_range_adaptor_copy_alg
 7/25 Test  #7: transform_view_.copy_func_empty_range_adaptor_copy_alg .....   Passed    0.00 sec
      Start  8: transform_view_.copy_func_adaptor_copy_alg
 8/25 Test  #8: transform_view_.copy_func_adaptor_copy_alg .................   Passed    0.00 sec
      Start  9: transform_view_.copy_func_empty_range_adaptor_to_vec
 9/25 Test  #9: transform_view_.copy_func_empty_range_adaptor_to_vec .......   Passed    0.00 sec
      Start 10: transform_view_.copy_func_adaptor_to_vec
10/25 Test #10: transform_view_.copy_func_adaptor_to_vec ...................   Passed    0.00 sec
      Start 11: transform_view_.lower_func_copy_alg
11/25 Test #11: transform_view_.lower_func_copy_alg ........................   Passed    0.00 sec
      Start 12: transform_view_.lower_func_range_adaptor
12/25 Test #12: transform_view_.lower_func_range_adaptor ...................   Passed    0.00 sec
      Start 13: transform_view_.lower_func_adaptor_to_vec
13/25 Test #13: transform_view_.lower_func_adaptor_to_vec ..................   Passed    0.00 sec
      Start 14: transform_view_.lower_upper_func_copy_alg
14/25 Test #14: transform_view_.lower_upper_func_copy_alg ..................   Passed    0.00 sec
      Start 15: transform_view_.lower_upper_func_range_adaptor
15/25 Test #15: transform_view_.lower_upper_func_range_adaptor .............   Passed    0.00 sec
      Start 16: transform_view_.lower_upper_func_adaptor_to_vec
16/25 Test #16: transform_view_.lower_upper_func_adaptor_to_vec ............   Passed    0.00 sec
      Start 17: transform_view_.sentinel_lower_func_copy_alg
17/25 Test #17: transform_view_.sentinel_lower_func_copy_alg ...............   Passed    0.00 sec
      Start 18: transform_view_.sentinel_lower_func_range_adaptor
18/25 Test #18: transform_view_.sentinel_lower_func_range_adaptor ..........   Passed    0.00 sec
      Start 19: transform_view_.sentinel_lower_func_adaptor_to_vec
19/25 Test #19: transform_view_.sentinel_lower_func_adaptor_to_vec .........   Passed    0.00 sec
      Start 20: transform_view_.sentinel_lower_upper_func_copy_alg
20/25 Test #20: transform_view_.sentinel_lower_upper_func_copy_alg .........   Passed    0.00 sec
      Start 21: transform_view_.sentinel_lower_upper_func_range_adaptor
21/25 Test #21: transform_view_.sentinel_lower_upper_func_range_adaptor ....   Passed    0.00 sec
      Start 22: transform_view_.sentinel_lower_upper_func_adaptor_to_vec
22/25 Test #22: transform_view_.sentinel_lower_upper_func_adaptor_to_vec ...   Passed    0.00 sec
      Start 23: transform_view_.borrowability
23/25 Test #23: transform_view_.borrowability ..............................   Passed    0.00 sec
      Start 24: transform_view_.borrowability_safety
24/25 Test #24: transform_view_.borrowability_safety .......................   Passed    0.00 sec
      Start 25: transform_view_.borrowability_safety_failure
25/25 Test #25: transform_view_.borrowability_safety_failure ...............   Passed    0.00 sec

100% tests passed, 0 tests failed out of 25

Total Test time (real) =   0.04 sec

# Run examples.
$ build/gcc-release/examples/beman.transform_view.examples.transform_view_direct_usage
lower

```

</details>

<details>
<summary> Install beman.transform_view (verbose logs) </summary>

```shell
# Install build artifacts from `build` directory into `opt/beman.transform_view` path.
$ cmake --install build/gcc-release --prefix /opt/beman.transform_view
-- Install configuration: "RelWithDebInfo"
-- Installing: /opt/beman.transform_view/lib/libbeman.transform_view.a
-- Installing: /opt/beman.transform_view/include/beman/transform_view/transform_view.hpp

# Check tree.
$ tree /opt/beman.transform_view
/opt/beman.transform_view
├── include
│   └── beman
│       └── transform_view
│           └── transform_view.hpp
└── lib
    └── libbeman.transform_view.a

5 directories, 2 files
```

</details>

<details>
<summary> Disable tests build </summary>

To build this project with tests disabled (and their dependencies),
simply use `BEMAN_TRANSFORM_VIEW_BUILD_TESTING=OFF` as documented in upstream [CMake documentation](https://cmake.org/cmake/help/latest/module/CTest.html):

```shell
cmake -B build -S . -DBEMAN_TRANSFORM_VIEW_BUILD_TESTING=OFF
```

</details>

## Integrate beman.transform_view into your project

<details>
<summary> Use beman.transform_view directly from C++ </summary>

This library is header only.  If you want to use `beman.transform_view` from your
project, you can include `beman/transform_view/*.hpp` files from your C++ source
files

```cpp
#include <beman/transform_view/transform_view.hpp>
```

and directly link with `libbeman.transform_view.a`

```shell
# Assume /opt/beman.transform_view staging directory.
$ c++ -o transform_view_usage examples/transform_view_direct_usage.cpp \
    -I /opt/beman.transform_view/include/ \
    -L/opt/beman.transform_view/lib/ -lbeman.transform_view
```

</details>

<details>
<summary> Use beman.transform_view directly from CMake </summary>

<!-- TODO Darius: rewrite section! Add examples. -->

For CMake based projects, you will need to use the `beman.transform_view` CMake module to define the `beman::transform_view` CMake target:

```cmake
find_package(beman.transform_view REQUIRED)
```

You will also need to add `beman::transform_view`
to the link libraries of any libraries or executables that include `beman/transform_view/*.hpp` in their source or header file.

```cmake
target_link_libraries(yourlib PUBLIC beman::transform_view)
```

</details>

<details>
<summary> Use beman.transform_view from other build systems </summary>

<!-- TODO Darius: rewrite section! Add examples. -->

Build systems that support `pkg-config` by providing a `beman.transform_view.pc` file.
Build systems that support interoperation via `pkg-config` should be able to detect `beman.transform_view` for you automatically.

</details>

### Compiler support

GCC 14 or later; Clang 18 or later; or VS 2022 or later.

Building this repository requires **C++23** or later.

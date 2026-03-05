# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.28...4.3)

# TODO: Set BEMAN_USE_MODULES if one of a particular set of compiler versions is being used

if(BEMAN_USE_MODULES)
    set(CMAKE_CXX_SCAN_FOR_MODULES ON)
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD
        "d0edc3af-4c50-42ea-a356-e2862fe7a444"
    )
endif()

project(
    beman.transform_view # CMake Project Name, which is also the name of the top-level
    # targets (e.g., library, executable, etc.).
    DESCRIPTION "A conditionally borrowed `std::ranges::transform_view`"
    LANGUAGES CXX
    VERSION 0.1.0
)

# [CMAKE.SKIP_TESTS]
option(
    BEMAN_TRANSFORM_VIEW_BUILD_TESTS
    "Enable building tests and test infrastructure. Default: ${PROJECT_IS_TOP_LEVEL}. Values: { ON, OFF }."
    ${PROJECT_IS_TOP_LEVEL}
)

# [CMAKE.SKIP_EXAMPLES]
option(
    BEMAN_TRANSFORM_VIEW_BUILD_EXAMPLES
    "Enable building examples. Default: ${PROJECT_IS_TOP_LEVEL}. Values: { ON, OFF }."
    ${PROJECT_IS_TOP_LEVEL}
)

# for find of beman-install-library
include(infra/cmake/beman-install-library.cmake)

add_library(beman.transform_view INTERFACE)
add_library(beman::transform_view ALIAS beman.transform_view)

target_sources(beman.transform_view PUBLIC FILE_SET HEADERS BASE_DIRS include)

set_target_properties(
    beman.transform_view
    PROPERTIES VERIFY_INTERFACE_HEADER_SETS ON
)

add_subdirectory(include/beman/transform_view)

if(BEMAN_USE_MODULES)
    add_library(beman.transform_view_module STATIC)
    add_library(beman::transform_view_module ALIAS beman.transform_view_module)

    if(BEMAN_USE_STD_MODULES)
        target_compile_features(beman.transform_view_module PUBLIC cxx_std_23)
        # XXX set_target_properties(beman.transform_view_module PROPERTIES CXX_MODULE_STD ON)
    endif()

    target_sources(
        beman.transform_view_module
        PUBLIC FILE_SET HEADERS BASE_DIRS include
    )

    add_subdirectory(src/beman/transform_view)
endif()

# NOT needed! include(CTest)
enable_testing()

beman_install_library(beman.transform_view TARGETS beman.transform_view beman.transform_view_module)

if(BEMAN_TRANSFORM_VIEW_BUILD_TESTS)
    add_subdirectory(tests/beman/transform_view)
endif()

if(BEMAN_TRANSFORM_VIEW_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

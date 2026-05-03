# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.30...4.3)

include(infra/cmake/enable-experimental-import-std.cmake)

project(
    beman.transform_view
    DESCRIPTION "A conditionally borrowed std::ranges::transform_view"
    LANGUAGES CXX
    VERSION 0.2.0
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

option(
    BEMAN_TRANSFORM_VIEW_USE_MODULES
    "Provide beman.transform_view as a C++ module"
    OFF
)

if(BEMAN_TRANSFORM_VIEW_USE_MODULES)
    set(CMAKE_CXX_SCAN_FOR_MODULES ON)
endif()

configure_file(
    "${PROJECT_SOURCE_DIR}/include/beman/transform_view/config_generated.hpp.in"
    "${PROJECT_BINARY_DIR}/include/beman/transform_view/config_generated.hpp"
    @ONLY
)

# for find of beman_install_library and configure_build_telemetry
include(infra/cmake/beman-install-library.cmake)
include(infra/cmake/BuildTelemetryConfig.cmake)

if(BEMAN_TRANSFORM_VIEW_USE_MODULES)
    add_library(beman.transform_view STATIC)
else()
    add_library(beman.transform_view INTERFACE)
endif()
add_library(beman::transform_view ALIAS beman.transform_view)

if(BEMAN_TRANSFORM_VIEW_USE_MODULES)
    target_sources(
        beman.transform_view
        PUBLIC
            FILE_SET CXX_MODULES
            FILE_SET HEADERS
                BASE_DIRS
                    "${CMAKE_CURRENT_SOURCE_DIR}/include"
                    "${CMAKE_CURRENT_BINARY_DIR}/include"
    )
    set_target_properties(beman.transform_view PROPERTIES CXX_MODULE_STD ON)
    target_compile_features(beman.transform_view PUBLIC cxx_std_23)
else()
    target_sources(
        beman.transform_view
        PUBLIC
            FILE_SET HEADERS
                BASE_DIRS
                    "${CMAKE_CURRENT_SOURCE_DIR}/include"
                    "${CMAKE_CURRENT_BINARY_DIR}/include"
    )
    set_target_properties(
        beman.transform_view
        PROPERTIES VERIFY_INTERFACE_HEADER_SETS ${PROJECT_IS_TOP_LEVEL}
    )
endif()

add_subdirectory(include/beman/transform_view)

beman_install_library(beman.transform_view TARGETS beman.transform_view)
configure_build_telemetry()

if(BEMAN_TRANSFORM_VIEW_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests/beman/transform_view)
endif()

if(BEMAN_TRANSFORM_VIEW_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

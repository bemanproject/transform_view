# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

add_library(beman.transform_view.module STATIC)
add_library(beman::transform_view.module ALIAS beman.transform_view.module)

target_sources(
    beman.transform_view.module
    PUBLIC FILE_SET CXX_MODULES FILES transform_view.cppm
)

target_include_directories(
    beman.transform_view.module
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_compile_definitions(
  beman.transform_view.module
  PUBLIC BEMAN_HAS_MODULES)

set_target_properties(
    beman.transform_view.module
    PROPERTIES CXX_MODULE_STD ON
)

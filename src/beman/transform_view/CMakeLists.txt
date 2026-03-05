# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

target_sources(
    beman.transform_view_module
    PUBLIC FILE_SET CXX_MODULES FILES transform_view.cppm
)

target_compile_definitions(beman.transform_view_module PUBLIC BEMAN_HAS_MODULES)

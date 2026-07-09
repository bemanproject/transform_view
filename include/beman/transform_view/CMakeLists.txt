# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

if(BEMAN_TRANSFORM_VIEW_USE_MODULES)
    target_sources(
        beman.transform_view
        PUBLIC
            FILE_SET CXX_MODULES FILES transform_view.cppm
            FILE_SET HEADERS
                FILES
                    config.hpp
                    transform_view.hpp
                    "${PROJECT_BINARY_DIR}/include/beman/transform_view/config_generated.hpp"
    )
else()
    target_sources(
        beman.transform_view
        PUBLIC
            FILE_SET HEADERS
                FILES
                    config.hpp
                    transform_view.hpp
                    "${PROJECT_BINARY_DIR}/include/beman/transform_view/config_generated.hpp"
    )
endif()

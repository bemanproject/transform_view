# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.30...4.3)

include(../cmake/prelude.cmake)

project(beman.transform_view.example LANGUAGES CXX)

if(PROJECT_IS_TOP_LEVEL)
    include(../cmake/cxx-modules-rules.cmake)

    find_package(beman.transform_view 0.1.0 EXACT REQUIRED)

    # XXX option(BEMAN_TRANSFORM_VIEW_IMPORT_STD "Use import std;" ${BEMAN_HAS_IMPORT_STD})

    enable_testing()
endif()

set(ALL_EXAMPLES transform_view_direct_usage)

message("Examples to be built: ${ALL_EXAMPLES}")

foreach(example ${ALL_EXAMPLES})
    add_executable(beman.transform_view.examples.${example})
    target_sources(
        beman.transform_view.examples.${example}
        PRIVATE ${example}.cpp
    )
    if(BEMAN_USE_MODULES)
        target_compile_features(
            beman.transform_view.examples.${example}
            PUBLIC cxx_std_${CMAKE_CXX_STANDARD}
        )
        if(BEMAN_HAS_IMPORT_STD)
            set_target_properties(
                beman.transform_view.examples.${example}
                PROPERTIES CXX_MODULE_STD ON
            )
        endif()
        target_link_libraries(
            beman.transform_view.examples.${example}
            PRIVATE beman::transform_view_module
        )
    else()
        target_link_libraries(
            beman.transform_view.examples.${example}
            PRIVATE beman::transform_view
        )
    endif()
    add_test(
        NAME beman.transform_view.examples.${example}
        COMMAND beman.transform_view.examples.${example}
    )
endforeach()

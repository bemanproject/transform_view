# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
cmake_minimum_required(VERSION 3.28...4.3)

# TODO: Set BEMAN_USE_MODULES if one of a particular set of compiler versions is being used

if(BEMAN_USE_MODULES)
    set(CMAKE_CXX_SCAN_FOR_MODULES ON)
    set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD
        "d0edc3af-4c50-42ea-a356-e2862fe7a444"
    )
endif()

project(beman.transform_view.example LANGUAGES CXX)

if(PROJECT_IS_TOP_LEVEL)
    find_package(beman.transform_view 0.1.0 EXACT REQUIRED)
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
            PUBLIC cxx_std_23
        )
        # XXX set_target_properties(beman.transform_view.examples.${example} PROPERTIES CXX_MODULE_STD ON)
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

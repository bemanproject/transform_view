# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

find_package(GTest REQUIRED)

add_executable(beman.transform_view.tests.transform_view)
target_sources(
    beman.transform_view.tests.transform_view
    PRIVATE transform_view.test.cpp
)
target_link_libraries(
    beman.transform_view.tests.transform_view
    PRIVATE beman::transform_view GTest::gtest_main
)
if(BEMAN_TRANSFORM_VIEW_USE_MODULES)
    set_target_properties(
        beman.transform_view.tests.transform_view
        PROPERTIES CXX_MODULE_STD ON
    )
endif()

include(GoogleTest)
gtest_discover_tests(
    beman.transform_view.tests.transform_view
    DISCOVERY_TIMEOUT 60
)

# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

find_package(GTest REQUIRED)

add_executable(beman.transform_view.tests.transform_view)
target_sources(beman.transform_view.tests.transform_view PRIVATE transform_view.test.cpp)
target_link_libraries(
    beman.transform_view.tests.transform_view
    PRIVATE beman::transform_view GTest::gtest GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(beman.transform_view.tests.transform_view)

# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

include(GoogleTest)

add_executable(beman.transform_view_26.tests.transform_view)
target_sources(
    beman.transform_view_26.tests.transform_view
    PRIVATE transform_view.test.cpp
)
target_link_libraries(
    beman.transform_view_26.tests.transform_view
    PRIVATE beman::transform_view_26 GTest::gtest GTest::gtest_main
)

gtest_add_tests(beman.transform_view_26.tests.transform_view "" AUTO)

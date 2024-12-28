# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

include(GoogleTest)

add_executable(beman.transform_view_26.tests.identity)
target_sources(beman.transform_view_26.tests.identity PRIVATE identity.test.cpp)
target_link_libraries(
    beman.transform_view_26.tests.identity
    PRIVATE beman::transform_view_26 GTest::gtest GTest::gtest_main
)

gtest_add_tests(beman.transform_view_26.tests.identity "" AUTO)

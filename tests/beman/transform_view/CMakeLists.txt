# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

find_package(GTest CONFIG REQUIRED)

add_executable(beman.transform_view.tests.transform_view)
target_sources(
    beman.transform_view.tests.transform_view
    PRIVATE transform_view.test.cpp
)
target_link_libraries(
    beman.transform_view.tests.transform_view
    PRIVATE beman::transform_view GTest::gtest GTest::gtest_main
)

if(BEMAN_USE_MODULES)
    add_executable(beman.transform_view.tests.module_smoke_test)
    target_sources(
        beman.transform_view.tests.module_smoke_test
        PRIVATE module_smoke_test.test.cpp
    )
    target_link_libraries(
        beman.transform_view.tests.module_smoke_test
        PRIVATE beman::transform_view_module
    )
endif()

include(GoogleTest)
gtest_discover_tests(beman.transform_view.tests.transform_view)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    return()
endif()

#======================================================================
# NOTE: Only for Release build with enabled install rules useful! CK
#======================================================================
if(
    BEMAN_TRANSFORM_VIEW_INSTALL_CONFIG_FILE_PACKAGE
    AND NOT CMAKE_SKIP_INSTALL_RULES
)
    # test if the targets are usable from the install directory
    add_test(
        NAME install-to-stagedir
        COMMAND
            ${CMAKE_COMMAND} --install ${CMAKE_BINARY_DIR} --prefix
            ${CMAKE_BINARY_DIR}/stagedir --config $<CONFIG>
    )
    add_test(
        NAME find-package-test
        COMMAND
            ${CMAKE_CTEST_COMMAND} # --verbose
            --output-on-failure -C $<CONFIG> #
            --build-and-test "${CMAKE_SOURCE_DIR}/examples"
            "${CMAKE_CURRENT_BINARY_DIR}/find-package-test" #
            --build-generator ${CMAKE_GENERATOR} #
            --build-makeprogram ${CMAKE_MAKE_PROGRAM} #
            --build-options #
            "-D BEMAN_USE_MODULES=${BEMAN_USE_MODULES}"
            "-D BEMAN_USE_STD_MODULE=${BEMAN_USE_STD_MODULE}"
            "-D CMAKE_BUILD_TYPE=$<CONFIG>"
            "-D CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}"
            "-D CMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS}"
            "-D CMAKE_CXX_MODULE_STD=${CMAKE_CXX_MODULE_STD}"
            "-D CMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}"
            "-D CMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED}"
            "-D CMAKE_PREFIX_PATH=${CMAKE_BINARY_DIR}/stagedir"
            "-D CMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}"
    )
endif()

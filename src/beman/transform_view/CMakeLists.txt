# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

include(GNUInstallDirs)

add_library(beman.transform_view)
add_library(beman::transform_view ALIAS beman.transform_view)

target_sources(beman.transform_view PRIVATE transform_view.cpp)

target_sources(
    beman.transform_view
    PUBLIC
        FILE_SET HEADERS
            BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../../../include
            FILES
                ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/beman/transform_view/transform_view.hpp
)

set_target_properties(
    beman.transform_view
    PROPERTIES VERIFY_INTERFACE_HEADER_SETS ON EXPORT_NAME transform_view
)

install(
    TARGETS beman.transform_view
    COMPONENT beman.transform_view
    EXPORT beman.transform_view
    DESTINATION ${CMAKE_INSTALL_LIBDIR}$<$<CONFIG:Debug>:/debug>
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}$<$<CONFIG:Debug>:/debug>
    FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

if(BEMAN_TRANSFORM_VIEW_INSTALL_CONFIG_FILE_PACKAGE)
    include(CMakePackageConfigHelpers)

    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/beman.transform_view-config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/beman.transform_view-config.cmake"
        INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/beman.transform_view"
        PATH_VARS PROJECT_NAME PROJECT_VERSION
    )

    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/beman.transform_view-config-version.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY ExactVersion
    )

    install(
        FILES
            "${CMAKE_CURRENT_BINARY_DIR}/beman.transform_view-config.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/beman.transform_view-config-version.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/beman.transform_view"
        COMPONENT beman.transform_view
    )

    install(
        EXPORT beman.transform_view
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/beman.transform_view"
        NAMESPACE beman::
        FILE beman.transform_view-targets.cmake
        COMPONENT beman.transform_view
    )
endif()

# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

add_library(beman.transform_view STATIC)
add_library(beman::transform_view ALIAS beman.transform_view)

target_sources(beman.transform_view PRIVATE transform_view.cpp)

target_sources(
    beman.transform_view
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${PROJECT_SOURCE_DIR}/include
        FILES
            ${PROJECT_SOURCE_DIR}/include/beman/transform_view/transform_view.hpp
)

set_target_properties(
    beman.transform_view
    PROPERTIES VERIFY_INTERFACE_HEADER_SETS ON
)

install(
    TARGETS beman.transform_view
    EXPORT beman.transform_view
    DESTINATION
    $<$<CONFIG:Debug>:debug/>${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION $<$<CONFIG:Debug>:debug/>${CMAKE_INSTALL_BINDIR}
    FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

add_library(beman.transform_view_26 STATIC)
add_library(beman::transform_view_26 ALIAS beman.transform_view_26)

target_sources(beman.transform_view_26 PRIVATE identity.cpp)

target_sources(
    beman.transform_view_26
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${PROJECT_SOURCE_DIR}/include
        FILES ${PROJECT_SOURCE_DIR}/include/beman/transform_view_26/identity.hpp
)

set_target_properties(beman.transform_view_26 PROPERTIES VERIFY_INTERFACE_HEADER_SETS ON)

install(
    TARGETS beman.transform_view_26
    EXPORT beman.transform_view_26
    DESTINATION
    $<$<CONFIG:Debug>:debug/>${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION $<$<CONFIG:Debug>:debug/>${CMAKE_INSTALL_BINDIR}
    FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

cmake_minimum_required(VERSION 3.21)
project(sap_profiling VERSION 0.1 LANGUAGES CXX)

if(TARGET sap::profiling)
    return()
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

option(SAP_PROFILING_BUILD_STATIC "Build static library" ON)
option(SAP_PROFILING_INSTALL "Install library" ON)

if(SAP_PROFILING_BUILD_STATIC)
    set(SAP_PROFILING_BUILD_SHARED OFF)
else()
    set(SAP_PROFILING_BUILD_SHARED ON)
endif()

set(SAP_CORE_BUILD_STATIC ${SAP_PROFILING_BUILD_STATIC})
set(SAP_CORE_BUILD_SHARED ${SAP_PROFILING_BUILD_SHARED})
add_subdirectory(sap_core)

set(SAP_PROFILING_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/sap_profiling/instrumentor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/sap_profiling/instrumentation_timer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/sap_profiling/utils.h
)

set(SAP_PROFILING_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/instrumentor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/instrumentation_timer.cpp
)

if(SAP_PROFILING_BUILD_SHARED)
    add_library(sap_profiling_shared SHARED 
        ${SAP_PROFILING_SOURCES}
    )
    target_include_directories(sap_profiling_shared
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
    )

    set_target_properties(sap_profiling_shared PROPERTIES
        OUTPUT_NAME sap_profiling
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
    )

    target_link_libraries(sap_profiling_shared PUBLIC sap::core)
    if(SAP_PROFILE)
        target_compile_definitions(sap_profiling_shared PUBLIC SAP_PROFILE=1)
    endif()
endif()

if(SAP_PROFILING_BUILD_STATIC)
    add_library(sap_profiling_static STATIC ${SAP_PROFILING_SOURCES})
    target_include_directories(sap_profiling_static
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
    )

    set_target_properties(sap_profiling_static PROPERTIES
        OUTPUT_NAME sap_profiling
        ARCHIVE_OUTPUT_NAME sap_profiling_static
    )

    target_link_libraries(sap_profiling_static PUBLIC sap::core)
    if(SAP_PROFILE)
        target_compile_definitions(sap_profiling_static PUBLIC SAP_PROFILE=1)
    endif()
endif()

if(SAP_PROFILING_BUILD_SHARED)
    add_library(sap::profiling ALIAS sap_profiling_shared)
elseif(SAP_PROFILING_BUILD_STATIC)
    add_library(sap::profiling ALIAS sap_profiling_static)
endif()


if(SAP_PROFILING_INSTALL)
    include(GNUInstallDirs)

    if(SAP_PROFILING_BUILD_SHARED)
        install(TARGETS sap_profiling_shared
            EXPORT sap_profiling_targets
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        )
    endif()

    if(SAP_PROFILING_BUILD_STATIC)
        install(TARGETS sap_profiling_static
            EXPORT sap_profiling_targets
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        )
    endif()

    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(EXPORT sap_profiling_targets
        FILE sap_profiling_targets.cmake
        NAMESPACE sap::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/sap_profiling
    )

    include(CMakePackageConfigHelpers)
    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/sap_profiling_config_version.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
    )

    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/sap_profiling_config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/sap_profiling_config.cmake"
        @ONLY
    )

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/sap_profiling_config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/sap_profiling_config_version.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/sap_profiling
    )
endif()
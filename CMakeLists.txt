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

set(SAP_PROFILING_BUILD_SHARED !${SAP_PROFILING_BUILD_STATIC})

set(SAP_CORE_BUILD_STATIC ${SAP_PROFILING_BUILD_STATIC})
set(SAP_CORE_BUILD_SHARED ${SAP_PROFILING_BUILD_SHARED})
include_directories(sap_core)

set(SAP_PROFILING_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/sap_profiling/types.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/sap_profiling/result.h
)

set(SAP_PROFILING_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/job_system.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/serialization.cpp)

if(SAP_PROFILING_BUILD_SHARED)
    add_library(sap_profiling_shared SHARED 
        ${SAP_PROFILING_SOURCES}
    )
    target_include_directories(sap_profiling_shared
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sap_core/include>
            $<INSTALL_INTERFACE:include>
            $<INSTALL_INTERFACE:sap_core/include>
    )

    set_target_properties(sap_profiling_shared PROPERTIES
        OUTPUT_NAME sap_profiling
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
    )
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

endif()

if(SAP_PROFILING_BUILD_SHARED)
    add_library(sap::profiling ALIAS sap_profiling_shared)
elseif(SAP_PROFILING_BUILD_STATIC)
    add_library(sap::profiling ALIAS sap_profiling_static)
endif()

target_link_libraries(sap::profiling PUBLIC sap::core)


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
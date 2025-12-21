cmake_minimum_required(VERSION 3.24) # support CMP0135

if(NOT DEFINED EPUZZLE_VERSION)
    set(EPUZZLE_VERSION "0.0.0") # no cahce
endif()

project(epuzzle 
    DESCRIPTION "A puzzle solver for logic puzzles (e.g., Einstein's/Zebra puzzle)"
    LANGUAGES CXX
    VERSION ${EPUZZLE_VERSION}
)
message(STATUS "Configuring ${PROJECT_NAME} version ${PROJECT_VERSION}")

configure_file(
    "${CMAKE_SOURCE_DIR}/cmake/version.h.in"
    "${CMAKE_BINARY_DIR}/generated/version.h"
) # @PROJECT_VERSION@ from version.h.in will be replaced with the real version in generated/version.h

include(FetchContent)
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/apps)
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/pdb)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)

# debug info for all configurations for easy diagnostic, static runtime for MSVC
set(EPUZZLE_STATIC_RUNTIME OFF)
if(MSVC)
    set(EPUZZLE_STATIC_RUNTIME ON)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    add_compile_options(/W4 /permissive- /utf-8 /Zi /Oy-)
    add_link_options(/DEBUG)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -finput-charset=UTF-8 -fexec-charset=UTF-8 -g -fno-omit-frame-pointer)
endif()

option(EPUZZLE_BUILD_APPS "Build application" ON)
option(EPUZZLE_BUILD_TESTING "Build tests" ON)

add_subdirectory(third_party)
add_subdirectory(utils)
add_subdirectory(core)

message(STATUS "Tests status: ${EPUZZLE_BUILD_TESTING}")
if(EPUZZLE_BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

message(STATUS "Application status: ${EPUZZLE_BUILD_APPS}")
if(EPUZZLE_BUILD_APPS)
    add_subdirectory(apps)

    set(CPACK_PACKAGE_NAME "epuzzle")
    set(CPACK_PACKAGE_VENDOR "mz-pilot")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Logic puzzle solver")
    set(CPACK_PACKAGE_DESCRIPTION "CLI tool for solving logic puzzles (e.g., Einstein's/Zebra puzzle)")
    set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
    set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
    set(CPACK_PACKAGE_CONTACT "mzexe@mail.ru")

    if (WIN32)
        set(CPACK_GENERATOR "ZIP")
    elseif(UNIX)
        set(CPACK_GENERATOR "TGZ;DEB")
        set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
        set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
    endif()

    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(EPUZZLE_ARCH "x64")
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(EPUZZLE_ARCH "x86")
    endif()

    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}-${EPUZZLE_ARCH}")

    include(CPack)
endif()

message(STATUS "Configuration complete. Project: ${PROJECT_NAME} ${PROJECT_VERSION}")
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(POLICY CMP0092)
    cmake_policy(SET CMP0092 NEW) # Don't add -W3 warning level by default.
endif()


file(READ "${CMAKE_CURRENT_SOURCE_DIR}/version.txt" VERSION_FILE_CONTENTS)
string(STRIP "${VERSION_FILE_CONTENTS}" VERSION_FILE_CONTENTS)
set(DYNHOOK_VERSION "${VERSION_FILE_CONTENTS}" CACHE STRING "Set version name")
set(DYNHOOK_PACKAGE "dynhook" CACHE STRING "Set package name")
string(REPLACE "v" "" DYNHOOK_VERSION "${DYNHOOK_VERSION}")
string(REGEX REPLACE "[.+-]" ";" DYNHOOK_VERSION_LIST ${DYNHOOK_VERSION})
list(GET DYNHOOK_VERSION_LIST 0 DYNHOOK_VERSION_MAJOR)
list(GET DYNHOOK_VERSION_LIST 1 DYNHOOK_VERSION_MINOR)
list(GET DYNHOOK_VERSION_LIST 2 DYNHOOK_VERSION_PATCH)

project(dynh 
        VERSION "${DYNHOOK_VERSION_MAJOR}.${DYNHOOK_VERSION_MINOR}.${DYNHOOK_VERSION_PATCH}"
        DESCRIPTION "DynHook Plugin" 
        HOMEPAGE_URL "https://github.com/untrustedmodders/dynhook" 
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT CMAKE_BUILD_TYPE MATCHES "Debug|Devel|MinSizeRel|RelWithDebInfo|Release")
    message(STATUS "CMAKE_BUILD_TYPE not set, defaulting to Debug.")
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

#
# Format
#
include(CompatFormat)

#
# Dynohook
#
add_subdirectory(external/dynohook)

#
# Plugin
#
file(GLOB_RECURSE PLUGIN_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp")

add_library(${PROJECT_NAME} SHARED ${PLUGIN_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} external/dynohook/include)

target_link_libraries(${PROJECT_NAME} PRIVATE dynohook)

if(MSVC)
    #target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    #target_compile_options(${PROJECT_NAME} PRIVATE -Wextra -Wshadow -Wconversion -Wpedantic -Werror)
endif()

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME} EXPORT_MACRO_NAME PLUGIN_API EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/exports/plugin_export.h)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/exports)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "dynhook")

if(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-Wl,-exported_symbols_list,${CMAKE_CURRENT_SOURCE_DIR}/sym/exported_symbols.lds")
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-Wl,--version-script,${CMAKE_CURRENT_SOURCE_DIR}/sym/version_script.lds")
endif()

if(LINUX)
    target_link_libraries(${PROJECT_NAME} PRIVATE -static-libstdc++ -static-libgcc)
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE 
        PLUGIFY_FORMAT_SUPPORT=$<BOOL:${COMPILER_SUPPORTS_FORMAT}> 
        PLUGIFY_IS_DEBUG=$<STREQUAL:${CMAKE_BUILD_TYPE},Debug>)
if(NOT COMPILER_SUPPORTS_FORMAT)
    target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt-header-only)
endif()

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/dynhook.pplugin.in
    ${CMAKE_CURRENT_BINARY_DIR}/dynhook.pplugin
)
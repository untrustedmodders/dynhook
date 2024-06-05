cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

if(POLICY CMP0092)
    cmake_policy(SET CMP0092 NEW) # Don't add -W3 warning level by default.
endif()


project(dyno VERSION 1.0.0.0 DESCRIPTION "DynoHook Plugin" HOMEPAGE_URL "https://github.com/untrustedmodders/dynohook" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

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

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "dynohook")

if(APPLE)
	target_link_libraries(${PROJECT_NAME} PRIVATE "-Wl,-exported_symbols_list,${CMAKE_CURRENT_SOURCE_DIR}/sym/exported_symbols.lds")
elseif(UNIX)
	target_link_libraries(${PROJECT_NAME} PRIVATE "-Wl,--version-script,${CMAKE_CURRENT_SOURCE_DIR}/sym/version_script.lds")
endif()

if(UNIX AND NOT APPLE)
    target_compile_definitions(${PROJECT_NAME} PUBLIC _GLIBCXX_USE_CXX11_ABI=0)
endif()

set(DYNOHOOK_VERSION "${CMAKE_PROJECT_VERSION}" CACHE STRING "Set version name")
set(DYNOHOOK_PACKAGE "dynohook" CACHE STRING "Set package name")

configure_file(
	${CMAKE_SOURCE_DIR}/dynohook.pplugin.in
	${CMAKE_BINARY_DIR}/dynohook.pplugin
)
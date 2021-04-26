cmake_minimum_required(VERSION 3.13)
project(DPLL LANGUAGES C)

# For multi-config generatores possible builds types are "Debug" and "Release"
if(DEFINED CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES Debug Release)
endif()

# Default build is a debug build
set(default_build_type "Debug")
if(NOT DEFINED CMAKE_CONFIGURATION_TYPES)
    # Set default build for single config generators
    if(CMAKE_BUILD_TYPE STREQUAL "")
        set(CMAKE_BUILD_TYPE "${default_build_type}"
            CACHE
            STRING "Choose type of build (Debug/Release)"
            FORCE)
    endif()
elseif(DEFINED CMAKE_CONFIGURATION_TYPES)
    # Set default build for ninja multi-config generator
    if (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.17")
        set(CMAKE_DEFAULT_BUILD_TYPE "${default_build_type}")
    endif()
endif()
unset(default_build_type)

# Correct CMAKE_BUILD_TYPE string to start with capital letter
if(DEFINED CMAKE_BUILD_TYPE)
    string(SUBSTRING ${CMAKE_BUILD_TYPE} 0  1 CMAKE_BUILD_TYPE_HEAD)
    string(SUBSTRING ${CMAKE_BUILD_TYPE} 1 -1 CMAKE_BUILD_TYPE_BODY)
    string(TOUPPER ${CMAKE_BUILD_TYPE_HEAD} CMAKE_BUILD_TYPE_HEAD)
    string(TOLOWER ${CMAKE_BUILD_TYPE_BODY} CMAKE_BUILD_TYPE_BODY)
    string(CONCAT CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE_HEAD} ${CMAKE_BUILD_TYPE_BODY})
    unset(CMAKE_BUILD_TYPE_HEAD)
    unset(CMAKE_BUILD_TYPE_BODY)
endif()

# Set default compiler options
set(CMAKE_C_STANDARD 99)
add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-pedantic)

option(
    BUILD_HELPER_AS_SHARED_LIB
    "Whether to build internal helper libraries as shared libraries.
    Default is to build them as static libraries.
    (Only useful for quicker compile times while developing)"
    OFF
)

# Define main executable
add_executable(dpll "")

add_subdirectory(libs)
add_subdirectory(src)

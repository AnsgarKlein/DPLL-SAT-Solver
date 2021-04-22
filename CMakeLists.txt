cmake_minimum_required(VERSION 3.13)
project(DPLL LANGUAGES C)

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

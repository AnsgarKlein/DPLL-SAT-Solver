# Create object library
set(objlibsources
    LinkedList.c
    StringBuilder.c
)
add_library(objlib OBJECT ${objlibsources})
set_target_properties(objlib PROPERTIES POSITION_INDEPENDENT_CODE 1)
target_include_directories(objlib PRIVATE "${CMAKE_CURRENT_LIST_DIR}/include")

# Create both static and shared library of object library.
# Creation of both libraries consists only of linking, because we first
# created an object library.
add_library(helper-static STATIC $<TARGET_OBJECTS:objlib>)
add_library(helper-shared SHARED $<TARGET_OBJECTS:objlib>)
set_target_properties(helper-shared PROPERTIES OUTPUT_NAME "helper")
set_target_properties(helper-static PROPERTIES OUTPUT_NAME "helper")

# Link with shared or static library depending on what is required for
# for this build.
if(BUILD_HELPER_AS_SHARED_LIB)
    target_link_libraries(dpll PRIVATE helper-shared)
else()
    target_link_libraries(dpll PRIVATE helper-static)
endif()

target_include_directories(dpll PRIVATE "${CMAKE_CURRENT_LIST_DIR}/include")

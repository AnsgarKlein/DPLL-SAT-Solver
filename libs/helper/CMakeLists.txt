# Create static library
add_library(helper STATIC
    LinkedList.c
    StringBuilder.c
)
target_include_directories(helper PRIVATE "${CMAKE_CURRENT_LIST_DIR}/include")

target_link_libraries(dpll PRIVATE helper)
target_include_directories(dpll PRIVATE "${CMAKE_CURRENT_LIST_DIR}/include")

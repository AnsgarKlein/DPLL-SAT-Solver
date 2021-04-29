set(test_resource_path "${CMAKE_CURRENT_LIST_DIR}/resources")
set(test_helper_path "${CMAKE_CURRENT_LIST_DIR}/helper")

add_subdirectory("functional")

unset(test_resource_path)
unset(test_helper_path)

foreach(i RANGE 1 4)
    set(i "0${i}")

    add_test(
        NAME "memory-test_basic${i}"
        COMMAND
        "${CMAKE_CURRENT_LIST_DIR}/test.sh"
        "$<TARGET_FILE:dpll>"
        "${test_resource_path}/sat/basic/basic-${i}.cnf"
    )
    set_tests_properties(
        "memory-test_basic${i}"
        PROPERTIES
        LABELS "memory"
    )
endforeach()

foreach(i RANGE 1 15)
    if(i LESS 10)
        set(i "000${i}")
    else()
        set(i "00${i}")
    endif()

    add_test(
        NAME "memory-test_supereasy${i}"
        COMMAND
        "${CMAKE_CURRENT_LIST_DIR}/test.sh"
        "$<TARGET_FILE:dpll>"
        "${test_resource_path}/sat/supereasy/supereasy-3sat-${i}.cnf"
    )
    set_tests_properties(
        "memory-test_supereasy${i}"
        PROPERTIES
        LABELS "memory"
    )
endforeach()

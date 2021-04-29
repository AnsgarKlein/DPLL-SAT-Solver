foreach(i RANGE 1 4)
    set(i "0${i}")

    add_test(
        NAME "functional-test_basic${i}"
        COMMAND
        "${CMAKE_CURRENT_LIST_DIR}/test_sat.sh"
        "$<TARGET_FILE:dpll>"
        "${test_resource_path}/sat/basic/basic-${i}.cnf"
        "${test_helper_path}/assignment_to_dimacs.py"
        "${test_helper_path}/solution_checker.py"
    )
    set_tests_properties(
        "functional-test_basic${i}"
        PROPERTIES
        LABELS "functional"
    )
endforeach()

foreach(i RANGE 1 15)
    if(i LESS 10)
        set(i "000${i}")
    else()
        set(i "00${i}")
    endif()

    add_test(
        NAME "functional-test_supereasy${i}"
        COMMAND
        "${CMAKE_CURRENT_LIST_DIR}/test_sat.sh"
        "$<TARGET_FILE:dpll>"
        "${test_resource_path}/sat/supereasy/supereasy-3sat-${i}.cnf"
        "${test_helper_path}/assignment_to_dimacs.py"
        "${test_helper_path}/solution_checker.py"
    )
    set_tests_properties(
        "functional-test_supereasy${i}"
        PROPERTIES
        LABELS "functional"
    )
endforeach()

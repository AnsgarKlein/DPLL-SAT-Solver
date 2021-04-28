#!/usr/bin/env python3

import sys

from evaluation import SAT, UNSAT, UNKNOWN

def parse_dimacs(lines):
    # Skip comments
    while lines[0][:2] == 'c ':
        assert len(lines) > 1
        lines = lines[1:]

    # Read problem line
    assert lines[0][:2] == 'p '
    assert lines[0].split(' ')[1] == 'cnf'
    num_literals = int(lines[0].split(' ')[2])
    num_clauses = int(lines[0].split(' ')[3])

    clauses = []
    literals = range(1, num_literals + 1)
    lines = lines[1:]

    # Parse all clauses
    for clause_line in lines:
        clause = clause_line.split(' ')

        # Each clause line must end with 0
        # (Not strictly correct but works most of the time)
        # (-> A clause can span multiple lines)
        assert int(clause[-1]) == 0
        clause = clause[:-1]

        clauses.append([int(lit) for lit in clause])

    # Check if cnf line was right
    assert len(clauses) == num_clauses
    assert len(literals) == num_literals

    return clauses, literals

def parse_solution(lines):
    answer = UNKNOWN
    model = set()

    for line in lines:
        # Skip comment lines
        if line[:2] == 'c ':
            continue

        # Read answer line
        if line[:2] == 's ':
            answer_str = line.split(' ')[1]
            if answer_str == 'SATISFIABLE':
                answer = SAT
            elif answer_str == 'UNSATISFIABLE':
                answer = UNSAT
            else:
                answer = UNKNOWN
        # Read value lines
        elif line[:2] == 'v ':
            value_elements = line[2:].split(' ')

            # Last element in value line has to be 0
            assert value_elements[-1] == '0'

            for element in value_elements:
                # Skip 0 elements
                if int(element) == 0:
                    continue

                if int(element) in model:
                    raise ValueError('Model cannot contain {} two times!'.format(
                        int(element)))
                if int(element) * -1 in model:
                    raise ValueError('Model cannot contain both {} and {}!'.format(
                        int(element),
                        int(element) * -1))
                model.add(int(element))

    return answer, model

if __name__ == '__main__':
    print('Error: Given module cannot be run directly', file = sys.stderr)
    sys.exit(1)

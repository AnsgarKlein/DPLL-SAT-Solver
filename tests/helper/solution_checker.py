#!/usr/bin/env python3

import argparse
import sys

from evaluation import SAT, UNSAT, UNKNOWN
from evaluation import correct_model
from parsing import parse_dimacs
from parsing import parse_solution

def read_input(path):
    # If path is '-' we read from stdin
    if path == '-':
        return [line.strip() for line in sys.stdin.readlines()]

    # Read from file
    with open(path, 'r') as f:
        return [line.strip() for line in f.readlines()]

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        '-p', '--problem',
        help = '''
            Path to file in DIMACS format specifying problem to solve.
            - for stdin.''',
        metavar = 'PATH',
        required = True,
        type = str)
    parser.add_argument(
        '-s', '--solution',
        help = '''
            Path to file in DIMACS output format specifying solution for given
            problem. - for stdin.''',
        metavar = 'PATH',
        required = True,
        type = str)
    args = parser.parse_args()

    # Read problem and solution
    cnf_lines = read_input(args.problem)
    solution_lines = read_input(args.solution)

    # Parse problem
    clauses, _ = parse_dimacs(cnf_lines)

    # Parse solution
    try:
        answer, model = parse_solution(solution_lines)
    except Exception as e:
        print('Error occured when parsing solution')
        print('Solution might be invalid. Error: {}'.format(str(e)))
        return 1

    # Check if answer was correct
    if answer == SAT:
        if correct_model(clauses, model):
            print('Given solution solves problem')
            return 0

        print('Given solution is not correct!')
        return 1

    print('Answer was not SAT. Can only check correctness of SAT answers')
    return 1

if __name__ == '__main__':
    sys.exit(main())

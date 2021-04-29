#!/usr/bin/env python3

###
### This helper script converts a given solution for a SAT problem in the
### following format:
###   3=false 1=false 9=true 6=true 2=false 7=true 4=false 10=true 5=? 8=true
###
### to the conventional DIMACS output format:
###   s SATISFIABLE
###   v -3 0
###   v -1 0
###   v 9 0
###   v 6 0
###   v -2 0
###   v 7 0
###   v -4 0
###   v 10 0
###   v 8 0
###
### The input is read from stdin and output is written to stdout.
### This script expects the input to be an assignment for a satisfiable formula.
###

import sys

def main():
    # Concatenate all lines
    input_str = ' '.join([line.strip() for line in sys.stdin.readlines()])
    print('c Converting assignments to DIMACS output format ...')

    # Extract all assignments
    assignment_strs = input_str.split(' ')
    print('c Assignments:')
    print('c {}'.format(input_str))
    print('c ')
    print('c Assuming satisfiable:')
    print('s SATISFIABLE')

    # Convert assignment str to dimacs model
    model = []
    for assignment_str in assignment_strs:
        variable = assignment_str.split('=')[0].strip()
        assignment = assignment_str.split('=')[1].strip()

        assert str.isdigit(variable)
        assert assignment in ('true', 'false', '?')

        if assignment == 'true':
            model.append(int(variable))
        elif assignment == 'false':
            model.append(int(variable) * -1)
        else:
            # Variable is neither true nor false
            # -> Variable does not matter
            # -> Don't mention variable in model
            pass

    # Output sorted model (for easier viewing)
    for var in sorted(model, key = abs):
        print('v {} 0'.format(var))

if __name__ == '__main__':
    sys.exit(main())

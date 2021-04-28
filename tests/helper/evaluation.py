#!/usr/bin/env python3

import sys

UNSAT   = 0
SAT     = 1
UNKNOWN = 2

def evaluate_clause(clause, model):
    # At least on literal has to be contained in model for clause to be SAT
    for literal in clause:
        if literal in model:
            return SAT

    # If at least one literal is not yet assigned clause can still be satisfied
    inverted_model = [lit * -1 for lit in model]
    for literal in clause:
        if literal not in inverted_model:
            return UNKNOWN

    # If every literal is already assigned (and not true) clause is UNSAT
    return UNSAT

def correct_model(clauses, model):
    # Model is correct if all clauses are true
    for clause in clauses:
        if evaluate_clause(clause, model) != SAT:
            return False
    return True

def incorrect_model(clauses, model):
    # Model is incorrect if one clause is false
    for clause in clauses:
        if evaluate_clause(clause, model) == UNSAT:
            return True
    return False

if __name__ == '__main__':
    print('Error: Given module cannot be run directly', file = sys.stderr)
    sys.exit(1)

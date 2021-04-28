#!/usr/bin/env python3

import sys

from evaluation import SAT, UNSAT, UNKNOWN
from evaluation import evaluate_clause
from evaluation import correct_model
from evaluation import incorrect_model
from parsing import parse_dimacs

DPLL_STEP_COUNTER = 0

def dpll(clauses, literals, verbose = False):
    if verbose:
        print('Literals:')
        for lit in literals:
            print('  {}'.format(lit))
        print('\nClauses:')
        for clause in clauses:
            print('  {}'.format(clause))

    result, result_model = dpll_step(clauses, literals, set(), verbose)

    # Output
    if verbose:
        print('c -----------------')
        print('c Steps:       {}'.format(DPLL_STEP_COUNTER))
        print('c DPLL Result: {}'.format(result))
        print('c Model: {}'.format(result_model))

    if result == SAT:
        print('s SATISFIABLE')

        # Print sorted model (for easier viewing)
        for lit in sorted(result_model, key = abs):
            print('v {} 0'.format(lit))
    elif result == UNSAT:
        print('s UNSATISFIABLE')
    else:
        print('s UNKNOWN')

def dpll_step(clauses, literals, model, verbose = False):
    global DPLL_STEP_COUNTER
    DPLL_STEP_COUNTER += 1

    if verbose:
        print('c Model: {}'.format(model))

    if correct_model(clauses, model):
        return True, model
    if incorrect_model(clauses, model):
        return False, model

    # Pick literal for assignment
    pick = pick_literal(clauses, literals, model, verbose = False)

    # Try positive assignment
    new_model = model.copy()
    new_model.add(pick)

    if verbose:
        print('c Model: {}'.format(new_model))

    unit_propagation(clauses, new_model, verbose = False)
    result, result_model = dpll_step(clauses, literals, new_model)
    if result:
        return result, result_model

    # Try negative assignment
    new_model = model.copy()
    new_model.add(pick * -1)

    if verbose:
        print('c Model: {}'.format(new_model))

    unit_propagation(clauses, new_model, verbose = False)
    result, result_model = dpll_step(clauses, literals, new_model)
    return result, result_model

def pick_literal(clauses, literals, model, verbose = False):
    # Look for pure literals
    lit = get_pure_literal(clauses, literals, model)
    if lit:
        if verbose:
            print('c picked pure literal {}'.format(lit))
        return lit

    # Just pick any literal
    for lit in literals:
        if lit not in model and (lit * -1) not in model:
            if verbose:
                print('c picked random literal {}'.format(lit))
            return lit

    # No unassigned literal found
    return None

def unit_propagation(clauses, model, verbose = False):
    def get_unit_literal(clause):
        # Minimize clause to only literals which are unassigned
        minimized_clause = [lit for lit in clause if lit not in model and (lit * -1) not in model]
        if len(minimized_clause) == 1:
            if verbose:
                print('c {} is a unit clause -> {}'.format(clause, minimized_clause[0]))
            return minimized_clause[0]
        return None

    unit_literals = []
    for clause in clauses:
        if evaluate_clause(clause, model) == SAT:
            continue
        unit_literal = get_unit_literal(clause)
        if unit_literal:
            unit_literals.append(unit_literal)

    #print('unit literals: {}'.format(unit_literals))

    unit_literals_added = set()

    for lit in unit_literals:
        if lit not in unit_literals_added and (lit * -1) not in unit_literals_added:
            unit_literals_added.add(lit)
            model.add(lit)

    if verbose:
        print('c unit literals: {}'.format(unit_literals_added))

def get_pure_literal(clauses, literals, model):
    def is_pure_literal(lit):
        contained_positive = False
        contained_negative = False

        positive_lit = lit
        negative_lit = lit * -1

        # Check if literal occurs only with one polarity
        for clause in clauses:
            # Minimize clause to only literals which are unassigned
            minimized_clause = [lit for lit in clause
                                if lit not in model
                                and (lit * -1) not in model]

            if positive_lit in minimized_clause:
                contained_positive = True
            elif negative_lit in minimized_clause:
                contained_negative = True

            # Pure literals occur only with one polarity
            if contained_positive and contained_negative:
                return False

        return True

    # Check if any unassigned literal is a pure literal
    unassigned_literals = (lit for lit in literals
                           if lit not in model
                           and (lit * -1) not in model)
    for lit in unassigned_literals:
        if is_pure_literal(lit):
            return lit

    # Found no pure literal
    return None

def main():
    # Read input
    lines = [line.strip() for line in sys.stdin.readlines()]
    clauses, literals = parse_dimacs(lines)

    dpll(clauses, literals)

if __name__ == '__main__':
    main()

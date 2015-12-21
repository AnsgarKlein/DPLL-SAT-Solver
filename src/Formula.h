/**
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/


#ifndef FORMULA_H
#define FORMULA_H


#include "Clause.h"
#include "LinkedList.h"


/**
 * Represents the possible status of a Formula.
 * 
 * TRUE - Formula is true
 * FALSE - Clause is false
 * UNDECIDED - Clause is not yet true or false
**/
typedef enum {
    FormulaStatus_TRUE,
    FormulaStatus_FALSE,
    FormulaStatus_UNDECIDED
} FormulaStatus;

/**
 * Represents a formula of propositional logic in conjunctive normal form.
 * 
 * A Formula contains Clauses, which contain Literals.
 * For a Formula to be true all contained Clauses have to be true.
**/
typedef struct {
    LinkedList* clauses;
    GenericLiteral** all_literals_v;
    unsigned int all_literals_c;
} Formula;


/**
 * Creates a new Formula.
**/
Formula* Formula_create(LinkedList* clauses, GenericLiteral** all_literals_v, unsigned int all_literals_c);

/**
 * Destroys a Formula.
**/
void Formula_destroy(Formula* formula);

/**
 * Represent this Formula as a string.
**/
char* Formula_to_string(Formula* formula, bool color);

/**
 * Represent current assignments of Literals as a string.
 * 
 * If print_all is false Literals that are not set will not be included
 * in the string.
**/
char* Formula_to_assignment_string(Formula* formula, bool print_all, bool color);

/**
 * Select Literal(s) (that are currently unassigned) from
 * One-Literal-Clauses to assign a value to.
 * 
 * Select unassigned Literals from One-Literal-Clauses along with
 * assignments which will most likely satisfy the Formula.
 * 
 * Returns NULL if no unassigned Literals from One-Literal-Clauses are left.
**/
LiteralAssignmentArray* Formula_unit_propagate(Formula* formula);

/**
 * Select next Literal (that is currently unassigned) to assign a
 * value to along with guess for best assignment.
 * 
 * Returns NULL if no unassigned Literals are left.
**/
LiteralAssignmentArray* Formula_choose_literal(Formula* formula);

FormulaStatus Formula_evaluate(Formula* formula);

bool Formula_dpll(Formula* formula);

#endif

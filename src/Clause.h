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


#ifndef CLAUSE_H
#define CLAUSE_H


#include "Literal.h"
#include "LinkedList.h"


/**
 * Represents the possible status of a Clause.
 *
 * TRUE - Clause is true
 * FALSE - Clause is false
 * UNDECIDED - Clause is not yet true or false
**/
typedef enum {
    ClauseStatus_TRUE,
    ClauseStatus_FALSE,
    ClauseStatus_UNDECIDED
} ClauseStatus;

/**
 * Represents a Clause of propositional logic in conjunctive normal form.
 *
 * A Clause contains Literals.
 * For a Clause to be true at least one contained Literal has to be true.
**/
typedef struct {
    Literal** literals_v;
    unsigned int literals_c;
    ClauseStatus clause_status;
} Clause;


/**
 * Creates a new Clause.
**/
Clause* Clause_create(Literal** literals_v, unsigned int literals_c);

/**
 * Destroys a Clause.
**/
void Clause_destroy(Clause* clause);

/**
 * Copy this object.
 *
 * Note: Contained Literals don't get copied!
**/
Clause* Clause_clone(Clause* clause);

/**
 * Represent Clause as a string.
**/
char* Clause_to_string(Clause* clause, bool color);

/**
 * Decides whether a given Clause is a unit Clause (only contains one single
 * unassigned Literal).
 *
 * If a given Clause is a unit Clause its only unassigned Literal is returned.
 * If a given Clause is not a unit Clause NULL is returned.
**/
Literal* Clause_is_unit_clause(Clause* clause);

/**
 * Evaluates a given Clause according to logical rules.
 *
 * Returns the status of a given Clause after evaluating all its Literals.
**/
ClauseStatus Clause_evaluate(Clause* clause);


#endif

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


#include "Clause.h"
#include "Constants.h"
#include "StringBuilder.h"

#include <assert.h>
#include <string.h>


Clause* Clause_create(Literal** literals_v, unsigned int literals_c) {
    assert(literals_v != NULL);
    
    Clause* clause = malloc(sizeof(Clause));
    assert(clause != NULL);
    
    clause->literals_v = literals_v;
    clause->literals_c = literals_c;
    clause->clause_status = ClauseStatus_UNDECIDED;
    
    return clause;
}

void Clause_destroy(Clause* clause) {
    assert(clause != NULL);
    
    // Free all Literals
    for (int i = 0; i < clause->literals_c; i++) {
        Literal_destroy(clause->literals_v[i]);
    }
    free(clause->literals_v);
    
    // Free Clause
    free(clause);
}

Clause* Clause_clone(Clause* clause) {
    // Clone array of Literals
    unsigned int count = clause->literals_c;
    Literal** cloned_array = malloc(count * sizeof(Literal*));
    assert(cloned_array != NULL);
    
    for (int i = 0; i < count; i++) {
        Literal* literal = clause->literals_v[i];
        cloned_array[i] = Literal_clone(literal);
    }
    
    // Create Clause from list of Literals
    return Clause_create(cloned_array, count);
}

char* Clause_to_string(Clause* clause, bool color) {
    assert(clause != NULL);
    
    StringBuilder* builder = StringBuilder_create(30);
    
    // If color is wanted add the correct color code to buffer
    if (color) {
        if (clause->clause_status == ClauseStatus_TRUE) {
            StringBuilder_append_string(builder, CONSTANTS_COLOR_PREFIX_TRUE);
        }
        else if (clause->clause_status == ClauseStatus_FALSE) {
            StringBuilder_append_string(builder, CONSTANTS_COLOR_PREFIX_FALSE);
        }
    }
    
    // Add symbol representing start of Clause
    StringBuilder_append_string(builder, CONSTANTS_CLAUSE_START);
    
    // If color is wanted add the default color code after the name
    if (color && clause->clause_status != ClauseStatus_UNDECIDED) {
        StringBuilder_append_string(builder, CONSTANTS_COLOR_SUFFIX);
    }
    
    // Add all Literals contained in this Clause
    for (int i = 0; i  < clause->literals_c; i++) {
        // Add Literal string
        char* lit_str = Literal_to_string(clause->literals_v[i], color);
        StringBuilder_append_string(builder, lit_str);
        free(lit_str);
        
        // Add delimiter between Literals
        if (i != clause->literals_c - 1) {
            StringBuilder_append_string(builder, CONSTANTS_LITERAL_DELIMITER);
        }
    }
    
    
    // If color is wanted add the correct color code to buffer
    if (color) {
        if (clause->clause_status == ClauseStatus_TRUE) {
            StringBuilder_append_string(builder, CONSTANTS_COLOR_PREFIX_TRUE);
        }
        else if (clause->clause_status == ClauseStatus_FALSE) {
            StringBuilder_append_string(builder, CONSTANTS_COLOR_PREFIX_FALSE);
        }
    }
    
    // Add symbol representing end of Clause
    StringBuilder_append_string(builder, CONSTANTS_CLAUSE_END);
    
    // If color is wanted add the default color code after the name
    if (color && clause->clause_status != ClauseStatus_UNDECIDED) {
        StringBuilder_append_string(builder, CONSTANTS_COLOR_SUFFIX);
    }
    
    return StringBuilder_destroy_to_string(builder);
}

Literal* Clause_is_unit_clause(Clause* clause) {
    assert(clause != NULL);
    
    // If all but one Literal are assigned return true.
    // If there are more than one unassigned Literal return false.
    bool found_unassigned_literal = false;
    Literal* only_literal = NULL;
    
    for (int i = 0; i < clause->literals_c; i++) {
        Literal* literal = clause->literals_v[i];
        LiteralAssignment assignment = GenericLiteral_get_assignment(literal->generic_literal);
        
        if (assignment == LiteralAssignment_UNSET) {
            // If we found an unassigned Literal previously there are
            // multiple unassigned Literals in this Clause.
            if (found_unassigned_literal) {
                return NULL;
            }
            
            found_unassigned_literal = true;
            only_literal = literal;
        }
    }
    
    return only_literal;
}

ClauseStatus Clause_evaluate(Clause* clause) {
    bool all_literals_are_false = true;
    
    // Check all Literals in this clause
    for (int i = 0; i < clause->literals_c; i++) {
        Literal* literal = clause->literals_v[i];
        LiteralAssignment assignment = GenericLiteral_get_assignment(literal->generic_literal);
        
        // If there are any Literals in this Clause that are UNSET
        // this Clause cannot be false
        if (assignment == LiteralAssignment_UNSET) {
            all_literals_are_false = false;
            continue;
        }
        
        if (assignment != LiteralAssignment_UNSET) {
            bool b = (assignment == LiteralAssignment_TRUE) ? true : false;
            
            // If any Literal is true this Clause is true
            if (b == !literal->negated) {
                clause->clause_status = ClauseStatus_TRUE;
                return clause->clause_status;
            } else {
                // If a Literal is false but there are other Literals in this
                // Clause that are true or unset everything is fine and this Clause
                // is not yet false.
            }
        }
    }
    
    // If all Literals are false this Clause is false.
    if (all_literals_are_false) {
        clause->clause_status = ClauseStatus_FALSE;
        return clause->clause_status;
    }
    
    // If this Clause is neither true nor false it is undecided.
    clause->clause_status = ClauseStatus_UNDECIDED;
    return clause->clause_status;
}

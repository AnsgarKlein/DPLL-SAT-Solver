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

#include <assert.h>
#include <string.h>


Clause* Clause_create(Literal** literals_v, uint literals_c) {
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
    uint count = clause->literals_c;
    
    Literal** cloned_array = malloc(count * sizeof(Literal*));
    assert(cloned_array != NULL);
    for (int i = 0; i < count; i++) {
        Literal* literal = clause->literals_v[i];
        cloned_array[i] = Literal_clone(literal);
    }
    
    Clause* cloned_clause = Clause_create(cloned_array, count);
    return cloned_clause;
}

char* Clause_to_string(Clause* clause, bool color) {
    assert(clause != NULL);
    
    uint buf_l = 10;
    char* buf = malloc(buf_l * sizeof(char));
    assert(buf != NULL);
    memset(buf, '\0', 1);
    
    // If color is wanted add the correct color code to buffer
    if (color) {
        if (clause->clause_status == ClauseStatus_TRUE) {
            // Resize if necessary and add string
            while ((strlen(buf) + strlen(CONSTANTS_COLOR_PREFIX_TRUE) + 1) > buf_l) {
                buf_l = buf_l * 2;
                buf = realloc(buf, buf_l);
            }
            strcat(buf, CONSTANTS_COLOR_PREFIX_TRUE);
        }
        else if (clause->clause_status == ClauseStatus_FALSE) {
            // Resize if necessary and add string
            while ((strlen(buf) + strlen(CONSTANTS_COLOR_PREFIX_FALSE) + 1) > buf_l) {
                buf_l = buf_l * 2;
                buf = realloc(buf, buf_l);
            }
            strcat(buf, CONSTANTS_COLOR_PREFIX_FALSE);
        }
    }
    
    // Add symbol representing start of Clause
    {
        // Resize if necessary and add string
        while ((strlen(buf) + strlen(CONSTANTS_CLAUSE_START) + 1) > buf_l) {
            buf_l = buf_l * 2;
            buf = realloc(buf, buf_l);
        }
        strcat(buf, CONSTANTS_CLAUSE_START);
    }
    
    // If color is wanted add the default color code after the name
    if (color && clause->clause_status != ClauseStatus_UNDECIDED) {
        // Resize if necessary and add string
        while ((strlen(buf) + strlen(CONSTANTS_COLOR_SUFFIX) + 1) > buf_l) {
            buf_l = buf_l * 2;
            buf = realloc(buf, buf_l);
        }
        strcat(buf, CONSTANTS_COLOR_SUFFIX);
    }
    
    // Add all Literals contained in this Clause
    for (int i = 0; i < clause->literals_c; i++) {
        // Add literal
        char* lit_str = Literal_to_string(clause->literals_v[i], color);
        
        // Resize if necessary and add string
        while ((strlen(buf) + strlen(lit_str) + 1) > buf_l) {
            buf_l = buf_l * 2;
            buf = realloc(buf, buf_l);
        }
        strcat(buf, lit_str);
        free(lit_str);
        
        // Add delimiter between Literals
        if (i != clause->literals_c - 1) {
            // Resize if necessary and add string
            while ((strlen(buf) + strlen(CONSTANTS_LITERAL_DELIMITER) + 1) > buf_l) {
                buf_l = buf_l * 2;
                buf = realloc(buf, buf_l);
            }
            strcat(buf, CONSTANTS_LITERAL_DELIMITER);
        }
    }
    
    
    // If color is wanted add the correct color code to buffer
    if (color) {
        if (clause->clause_status == ClauseStatus_TRUE) {
            // Resize if necessary and add string
            while ((strlen(buf) + strlen(CONSTANTS_COLOR_PREFIX_TRUE) + 1) > buf_l) {
                buf_l = buf_l * 2;
                buf = realloc(buf, buf_l);
            }
            strcat(buf, CONSTANTS_COLOR_PREFIX_TRUE);
        }
        else if (clause->clause_status == ClauseStatus_FALSE) {
            // Resize if necessary and add string
            while ((strlen(buf) + strlen(CONSTANTS_COLOR_PREFIX_FALSE) + 1) > buf_l) {
                buf_l = buf_l * 2;
                buf = realloc(buf, buf_l);
            }
            strcat(buf, CONSTANTS_COLOR_PREFIX_FALSE);
        }
    }
    
    // Add symbol representing end of Clause
    {
        // Resize if necessary and add string
        while ((strlen(buf) + strlen(CONSTANTS_CLAUSE_END) + 1) > buf_l) {
            buf_l = buf_l * 2;
            buf = realloc(buf, buf_l);
        }
        strcat(buf, CONSTANTS_CLAUSE_END);
    }
    
    // If color is wanted add the default color code after the name
    if (color && clause->clause_status != ClauseStatus_UNDECIDED) {
        // Resize if necessary and add string
        while ((strlen(buf) + strlen(CONSTANTS_COLOR_SUFFIX) + 1) > buf_l) {
            buf_l = buf_l * 2;
            buf = realloc(buf, buf_l);
        }
        strcat(buf, CONSTANTS_COLOR_SUFFIX);
    }
    
    // Shrink buffer to minimum required
    buf_l = (strlen(buf) + 1) * sizeof(char);
    buf = realloc(buf, buf_l);
    
    return buf;
}

Literal* Clause_is_unit_clause(Clause* clause) {
    assert(clause != NULL);
    
    // If all but one Literal are assigned return true.
    // If there are more than one unassigned Literal return false.
    bool found_unassigned_literal = false;
    Literal* only_literal = NULL;
    
    for (int i = 0; i < clause->literals_c; i++) {
        LiteralAssignment assignment = GenericLiteral_get_assignment(
                                       clause->literals_v[i]->generic_literal);
        if (assignment == LiteralAssignment_UNSET) {
            // If we found an unassigned Literal previously there are
            // multiple unassigned Literals in this Clause.
            if (found_unassigned_literal) {
                return NULL;
            }
            
            found_unassigned_literal = true;
            only_literal = clause->literals_v[i];
        }
    }
    
    return only_literal;
}

ClauseStatus Clause_evaluate(Clause* clause) {
    // Check all Literals in this clause
    for (int i = 0; i < clause->literals_c; i++) {
        Literal* literal = clause->literals_v[i];
        bool assignment = false;
        
        switch (GenericLiteral_get_assignment(literal->generic_literal)) {
        case LiteralAssignment_TRUE:
            assignment = true;
            break;
        case LiteralAssignment_FALSE:
            assignment = false;
            break;
        case LiteralAssignment_UNSET:
            continue;
            break;
        }
        
        // If any Literal is true this Clause is true.
        if (assignment == !literal->negated) {
            clause->clause_status = ClauseStatus_TRUE;
            return clause->clause_status;

        }
        
        // If a Literal is false:
        if (assignment == literal->negated) {
            // If a Literal is false but there are other Literals in this
            // Clause that are true or not set everything is fine.
            bool all_literals_false = true;
            
            // Check all Literals
            for (int p = 0; p < clause->literals_c; p++) {
                Literal* literal2 = clause->literals_v[p];
                LiteralAssignment assign = GenericLiteral_get_assignment(literal2->generic_literal);
                
                if ((assign == LiteralAssignment_UNSET) ||
                    (assign == LiteralAssignment_TRUE && !literal2->negated) ||
                    (assign == LiteralAssignment_FALSE && literal2->negated)) {
                    
                    all_literals_false = false;
                    break;
                }
            }
            
            // If there is only one Literal left in this Clause and it
            // is false this Clause is false.
            if (all_literals_false) {
                clause->clause_status = ClauseStatus_FALSE;
                return clause->clause_status;
            }
        }
    
    }
    
    // If this Clause is neither true nor false it is undecided.
    clause->clause_status = ClauseStatus_UNDECIDED;
    return clause->clause_status;
}

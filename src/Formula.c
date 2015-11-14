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


#include "Formula.h"
#include "Constants.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>


Formula* Formula_create(LinkedList* clauses, LinkedList* all_literals) {
    assert(clauses != NULL);
    assert(all_literals != NULL);
    
    Formula* formula = malloc(sizeof(Formula));
    assert(formula != NULL);
    
    formula->clauses = clauses;
    formula->all_literals = all_literals;
    
    return formula;
}

void Formula_destroy(Formula* formula) {
    assert(formula != NULL);
    
    // Free all Clauses
    LinkedList_destroy(formula->clauses, true);
    
    // Free all Literals
    LinkedList_destroy(formula->all_literals, true);
    
    // Free formula
    free(formula);
}

char* Formula_to_string(Formula* formula, bool color) {
    assert(formula != NULL);
    
    unsigned int buf_l = 10;
    char* buf = malloc(buf_l * sizeof(char));
    assert(buf != NULL);
    memset(buf, '\0', 1);
    
    
    // Add all Clauses contained in this Formula
    for (LinkedListNode* iter = formula->clauses->head; iter != NULL; iter = iter->next) {
        // Add Clause
        char* clause_str = Clause_to_string(iter->data, color);
        
        // Resize if necessary and add string
        while ((strlen(buf) + strlen(clause_str) + 1) > buf_l) {
            buf_l = buf_l * 2;
            buf = realloc(buf, buf_l);
        }
        strcat(buf, clause_str);
        free(clause_str);
        
        // Add delimiter between Clauses
        if (iter->next != NULL) {
            // Resize if necessary and add string
            while ((strlen(buf) + strlen(CONSTANTS_CLAUSE_DELIMITER) + 1) > buf_l) {
                buf_l = buf_l * 2;
                buf = realloc(buf, buf_l);
            }
            strcat(buf, CONSTANTS_CLAUSE_DELIMITER);
        }
    }
    
    // Shrink buffer to minimum required
    buf_l = (strlen(buf) + 1) * sizeof(char);
    buf = realloc(buf, buf_l);
    
    return buf;
}

char* Formula_to_assignment_string(Formula* formula, bool print_all, bool color) {
    assert(formula != NULL);
    
    unsigned int buf_l = 10;
    char* buf = malloc(buf_l * sizeof(char));
    assert(buf != NULL);
    memset(buf, '\0', 1);
    
    
    for (LinkedListNode* iter = formula->all_literals->head; iter != NULL; iter = iter->next) {
        GenericLiteral* literal = iter->data;
        
        if (print_all || GenericLiteral_get_assignment(literal) != LiteralAssignment_UNSET) {
            char* lit_str = GenericLiteral_to_assignment_string(literal, color);
            
            // Resize if necessary and add string
            while ((strlen(buf) + strlen(lit_str) + 1) > buf_l) {
                buf_l = buf_l * 2;
                buf = realloc(buf, buf_l);
            }
            strcat(buf, lit_str);
            free(lit_str);
            
            // Resize if necessary and add separator char
            if (strlen(buf) + 1 + 1 > buf_l) {
                buf_l = buf_l * 2;
                buf = realloc(buf, buf_l);
            }
            unsigned int len = strlen(buf);
            buf[len] = ' ';
            buf[len + 1] = '\0';
        }
    }
    
    
    // Shrink buffer to minimum required
    buf_l = (strlen(buf) + 1) * sizeof(char);
    buf = realloc(buf, buf_l);
    
    return buf;
}

LiteralAssignmentArray* Formula_unit_propagate(Formula* formula) {
    // If One-Literal-Clauses exists return their Literals,
    // with the appropriate assignments.
    
    unsigned int size = 3;
    unsigned int filled = 0;
    GenericLiteral** literals_v = malloc(size * sizeof(GenericLiteral*));
    bool* assignments = malloc(size * sizeof(bool));
    assert(literals_v != NULL);
    assert(assignments != NULL);
    
    for (LinkedListNode* iter = formula->clauses->head; iter != NULL; iter = iter->next) {
        Clause* cl = iter->data;
        Literal* only_literal = Clause_is_unit_clause(cl);
        
        if (only_literal == NULL) {
            continue;
        }
        
        #if VERBOSE_DPLL
            printf("  Unassigned Literal from One-Literal-Clause: %s\n",
                   GenericLiteral_get_name(only_literal->generic_literal));
        #endif
        
        // Resize if necessary
        if (filled + 1 > size) {
            size = size * 2;
            literals_v = realloc(literals_v, size * sizeof(Literal*));
            assignments = realloc(assignments, size * sizeof(bool));
        }
        
        // Add Literal and assignment to arrays
        literals_v[filled] = only_literal->generic_literal;
        if (only_literal->negated) {
            assignments[filled] = false;
        } else {
            assignments[filled] = true;
        }
        filled++;
    }
    
    // Shrink arrays to minimum size required
    size = filled;
    literals_v = realloc(literals_v, size * sizeof(Literal*));
    assignments = realloc(assignments, size * sizeof(bool));
    
    // If no appropriate Literals were found return false,
    // otherwise return true;
    if (filled == 0) {
        free(literals_v);
        free(assignments);
        literals_v = NULL;
        assignments = NULL;
        return NULL;
    }
    
    LiteralAssignmentArray* assignment_array;
    assignment_array = LiteralAssignmentArray_create(literals_v,
                                                     assignments,
                                                     size);
    
    return assignment_array;
}

LiteralAssignmentArray* Formula_choose_literal(Formula* formula) {
    #if VERBOSE_DPLL
    {
        // Create a array of all unset Literals
        unsigned int available_literals_size = 2;
        unsigned int available_literals_filled = 0;
        GenericLiteral** available_literals_v = malloc(
                        available_literals_size * sizeof(GenericLiteral*));
        assert(available_literals_v != NULL);
        
        for (LinkedListNode* iter = formula->all_literals->head; iter != NULL; iter = iter->next) {
            GenericLiteral* lit = iter->data;
            
            if (GenericLiteral_get_assignment(lit) == LiteralAssignment_UNSET) {
                // Resize if necessary
                if (available_literals_filled + 1 > available_literals_size) {
                    available_literals_size *= 2;
                    available_literals_v = realloc(
                                available_literals_v,
                                available_literals_size * sizeof(GenericLiteral*));
                }
                
                // Add to array
                available_literals_v[available_literals_filled++] = lit;
            }
        }
        
        // Print all literals (set and unset)
        {
            unsigned int buf_l = 30;
            char* buf = malloc(buf_l * sizeof(char));
            assert(buf != NULL);
            memset(buf, '\0', 1);
            strcat(buf, "All literals: (");
            
            int i = 0;
            for (LinkedListNode* iter = formula->all_literals->head; iter != NULL; iter = iter->next) {
                i++;
                GenericLiteral* literal = iter->data;
                char* lit_str = literal->name;
                
                // Resize if necessary and add literal string
                while (strlen(buf) + strlen(lit_str) + 1 > buf_l) {
                    buf_l = buf_l * 2;
                    buf = realloc(buf, buf_l);
                }
                strcat(buf, lit_str);
                
                // Add separator char
                if (i != formula->all_literals->size) {
                    // Resize if necessary and add separator char
                    if (strlen(buf) + 1 + 1 > buf_l) {
                        buf_l = buf_l * 2;
                        buf = realloc(buf, buf_l);
                    }
                    unsigned int len = strlen(buf);
                    buf[len] = ',';
                    buf[len + 1] = '\0';
                }
            }
            
            printf("  %s)\n", buf);
            free(buf);
        }
        
        // Print all available Literals
        {
            unsigned int buf_l = 30;
            char* buf = malloc(buf_l * sizeof(char));
            assert(buf != NULL);
            memset(buf, '\0', 1);
            strcat(buf, "Available literals: (");
            
            for (int i = 0; i < available_literals_filled; i++) {
                GenericLiteral* literal = available_literals_v[i];
                char* lit_str = literal->name;
                
                // Resize if necessary and add literal string
                while (strlen(buf) + strlen(lit_str) + 1 > buf_l) {
                    buf_l = buf_l * 2;
                    buf = realloc(buf, buf_l);
                }
                strcat(buf, lit_str);
                
                // Add separator char
                if (i != available_literals_filled - 1) {
                    // Resize if necessary and add separator char
                    if (strlen(buf) + 1 + 1 > buf_l) {
                        buf_l = buf_l * 2;
                        buf = realloc(buf, buf_l);
                    }
                    unsigned int len = strlen(buf);
                    buf[len] = ',';
                    buf[len + 1] = '\0';
                }
            }
            
            printf("  %s)\n", buf);
            free(buf);
        }
        
        // Free array of unset Literals
        free(available_literals_v);
    }
    #endif
    
    // Just return the first available (unset) Literal
    for (LinkedListNode* iter = formula->all_literals->head; iter != NULL; iter = iter->next) {
        GenericLiteral* lit = iter->data;
        
        if (GenericLiteral_get_assignment(lit) == LiteralAssignment_UNSET) {
            #if VERBOSE_DPLL
                printf("  Picked unassigned literal: %s\n", GenericLiteral_get_name(lit));
            #endif
            
            GenericLiteral** literals = malloc(1 * sizeof(GenericLiteral*));
            assert(literals != NULL);
            literals[0] = lit;
            
            bool* assignment = malloc(sizeof(bool));
            assert(assignment != NULL);
            *assignment = true;
            
            LiteralAssignmentArray* array;
            array = LiteralAssignmentArray_create(literals, assignment, 1);
            return array;
        }
    }
    
    // No literal left
    return NULL;
}

FormulaStatus Formula_evaluate(Formula* formula) {
    {
        // Evaluate current assignment
        #if VERBOSE_DPLL
            printf("  evaluating ...\n");
        #endif
        
        unsigned int true_clauses_size = 10;
        unsigned int true_clauses_filled = 0;
        Clause** true_clauses_v = malloc(true_clauses_size * sizeof(Clause*));
        assert(true_clauses_v != NULL);
        
        for (LinkedListNode* iter = formula->clauses->head; iter != NULL; iter = iter->next) {
            Clause* clause = iter->data;
            
            switch(Clause_evaluate(clause)) {
                case ClauseStatus_TRUE:
                    #if VERBOSE_DPLL
                    {
                        char* clause_str = Clause_to_string(clause, CONSTANTS_COLOR_ENABLED);
                        printf("  Clause %s is true, removing ...\n", clause_str);
                        free(clause_str);
                    }
                    #endif
                    
                    // Resize if necessary and add to array
                    if (true_clauses_filled + 1 > true_clauses_size) {
                        true_clauses_size *= 2;
                        true_clauses_v = realloc(true_clauses_v, true_clauses_size * sizeof(Clause*));
                    }
                    true_clauses_v[true_clauses_filled++] = clause;
                    break;
                case ClauseStatus_FALSE:
                    #if VERBOSE_DPLL
                    {
                        char* clause_str = Clause_to_string(clause, CONSTANTS_COLOR_ENABLED);
                        printf("  Clause %s is false, going back ...\n", clause_str);
                        free(clause_str);
                    }
                    #endif
                    
                    // If one Clause is false the whole Formual is false
                    // so we go back and try again.
                    free(true_clauses_v);
                    return FormulaStatus_FALSE;
                    break;
                case ClauseStatus_UNDECIDED:
                    break;
            }
        }
        
        // Remove all true Clauses from Formula
        for (int i = 0; i < true_clauses_filled; i++) {
            Clause* true_clause = true_clauses_v[i];
            
            // Remove Clause
            bool success = LinkedList_remove(formula->clauses, true_clause, true);
            assert(success == true);
        }
        
        free(true_clauses_v);
    }
    
    #if VERBOSE_DPLL
    {
        char* formula_str = Formula_to_string(formula, CONSTANTS_COLOR_ENABLED);
        char* formula_assign_str = Formula_to_assignment_string(formula, false, CONSTANTS_COLOR_ENABLED);
        
        printf("\n\n\n");
        printf("Formula:\t%s\n", formula_str);
        printf("Assignments:\t%s\n", formula_assign_str);
        
        free(formula_str);
        free(formula_assign_str);
    }
    #endif
    
    // Check if current assignment made Formula true
    if (formula->clauses->size == 0) {
    //if (ClauseList_length(formula->clauses) == 0) {
        #if VERBOSE_DPLL
            printf("  0 Clauses -> formula satisfied\n");
        #endif
        
        return FormulaStatus_TRUE;
    }
    
    return FormulaStatus_UNDECIDED;
}

bool Formula_dpll(Formula* formula) {
    #if VERBOSE_DPLL
    {
        char* formula_str = Formula_to_string(formula, CONSTANTS_COLOR_ENABLED);
        char* formula_assign_str = Formula_to_assignment_string(formula, false, CONSTANTS_COLOR_ENABLED);
        
        printf("\n\n\n");
        printf("Formula:\t%s\n", formula_str);
        printf("Assignments:\t%s\n", formula_assign_str);
        
        free(formula_str);
        free(formula_assign_str);
    }
    #endif
    
    // Evaluate Formula
    FormulaStatus status = Formula_evaluate(formula);
    if (status == FormulaStatus_TRUE) {
        return true;
    } else if (status == FormulaStatus_FALSE) {
        return false;
    }
    
    // Try to find another another Literal which hasn't been
    // assigned a value.
    //
    // Normally we first try to assign true to Literals, but sometimes
    // (a negated Literal in a One-Literal-Clause) it's smarter to
    // assign false first.
    LiteralAssignmentArray* assignment_array;
    assignment_array = Formula_unit_propagate(formula);
    
    if (assignment_array == NULL) {
        assignment_array = Formula_choose_literal(formula);
    }
    
    // If we don't find one it means every Literal has a value
    // in the current assignment, but the it doesn't make the Formula
    // true.
    // So we return false and try another assignment (or fail).
    if (assignment_array == NULL) {
        #if VERBOSE_DPLL
            printf("  Found no unassigned literal\n");
        #endif
        return false;
    }
    
    // Set the found Literal to the preferred assignment and rerun
    // the algorithm.
    for (int i = 0; i < 2; i++) {
        // Save state of Clauses
        LinkedList* cloned_list = LinkedList_create((void (*)(void*))&Clause_destroy);
        
        for (LinkedListNode* iter = formula->clauses->head; iter != NULL; iter = iter->next) {
            Clause* cloned_clause = Clause_clone(iter->data);
            
            LinkedList_prepend(cloned_list, cloned_clause);
        }
        
        // Set selected Literal to selected assignment
        for (int p = 0; p < assignment_array->size; p++) {
            GenericLiteral* lit = assignment_array->literals[p];
            bool assignment = assignment_array->assignments[p];
            
            GenericLiteral_assign(lit, assignment);
        }
        
        #if VERBOSE_DPLL
            for (int p = 0; p < assignment_array->size; p++) {
                GenericLiteral* lit = assignment_array->literals[p];
                char* assignment_str = NULL;
                assignment_str = GenericLiteral_to_assignment_string(lit, CONSTANTS_COLOR_ENABLED);
                printf("  Trying %s\n", assignment_str);
                free(assignment_str);
            }
        #endif
        
        if (Formula_dpll(formula)) {
            LiteralAssignmentArray_destroy(assignment_array);
            LinkedList_destroy(cloned_list, true);
            
            return true;
        }
        
        // If assignment wasn't correct restore state of Clauses from
        // before, set the Literal to the non-preferred assignment
        // and rerun.
        LinkedList_destroy(formula->clauses, true);
        formula->clauses = cloned_list;
        
        for (int p = 0; p < assignment_array->size; p++) {
            assignment_array->assignments[p] = !assignment_array->assignments[p];
        }
    }
    
    // If neither makes the Formula true, we will restore the state
    // of assignment from before false and return false.
    for (int i = 0; i < assignment_array->size; i++) {
        GenericLiteral* lit = assignment_array->literals[i];
        GenericLiteral_unassign(lit);
    }
    
    // The algorithm will rerun with another variable on an upper level.
    // (or it will just return false if the Formula is unsatisfiable)
    LiteralAssignmentArray_destroy(assignment_array);
    return false;
}


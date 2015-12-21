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
#include "StringBuilder.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>


Formula* Formula_create(LinkedList* clauses, GenericLiteral** all_literals_v, unsigned int all_literals_c) {
    assert(clauses != NULL);
    assert(all_literals_v != NULL);
    
    Formula* formula = malloc(sizeof(Formula));
    assert(formula != NULL);
    
    formula->clauses = clauses;
    formula->all_literals_v = all_literals_v;
    formula->all_literals_c = all_literals_c;
    
    return formula;
}

void Formula_destroy(Formula* formula) {
    assert(formula != NULL);
    
    // Free all Clauses
    LinkedList_destroy(formula->clauses, true);
    
    // Free all Literals
    for (int i = 0; i < formula->all_literals_c; i++) {
        GenericLiteral_destroy(formula->all_literals_v[i]);
    }
    free(formula->all_literals_v);
    
    // Free formula
    free(formula);
}

char* Formula_to_string(Formula* formula, bool color) {
    assert(formula != NULL);
    
    // Create string
    StringBuilder* builder = StringBuilder_create(10);
    
    // Add all Clauses contained in this Formula
    for (LinkedListNode* iter = formula->clauses->head; iter != NULL; iter = iter->next) {
        // Add Clause
        char* clause_str = Clause_to_string(iter->data, color);
        StringBuilder_append_string(builder, clause_str);
        free(clause_str);
        
        // Add delimiter between Clauses
        if (iter->next != NULL) {
            StringBuilder_append_string(builder, CONSTANTS_CLAUSE_DELIMITER);
        }
    }
    
    return StringBuilder_destroy_to_string(builder);
}

char* Formula_to_assignment_string(Formula* formula, bool print_all, bool color) {
    assert(formula != NULL);
    
    // Create string
    StringBuilder* builder = StringBuilder_create(100);
    
    // Add assignment string for all Literals
    for (int i = 0; i < formula->all_literals_c; i++) {
        GenericLiteral* literal = formula->all_literals_v[i];
        
        if (print_all || GenericLiteral_get_assignment(literal) != LiteralAssignment_UNSET) {
            // Add Literal assignment string
            char* lit_str = GenericLiteral_to_assignment_string(literal, color);
            StringBuilder_append_string(builder, lit_str);
            free(lit_str);
            
            // Add separator char
            StringBuilder_append_char(builder, ' ');
        }
    }
    
    return StringBuilder_destroy_to_string(builder);
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
        if (cl->clause_status != ClauseStatus_UNDECIDED) {
            continue;
        }
        
        Literal* only_literal = Clause_is_unit_clause(cl);
        if (only_literal == NULL) {
            continue;
        }
        
        #if VERBOSE_DPLL
            printf("  Unassigned Literal from One-Literal-Clause: %s\n",
                   only_literal->generic_literal->name);
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
        LinkedList* unset_literals = LinkedList_create((void(*)(void*))GenericLiteral_destroy);
        
        for (int i = 0; i < formula->all_literals_c; i++) {
            GenericLiteral* lit = formula->all_literals_v[i];
            
            if (GenericLiteral_get_assignment(lit) == LiteralAssignment_UNSET) {
                LinkedList_prepend(unset_literals, lit);
            }
        }
        
        // Print all literals (set and unset)
        {
            // Create string
            StringBuilder* builder = StringBuilder_create(100);
            StringBuilder_append_string(builder, "All literals: (");
            
            for (int i = 0; i < formula->all_literals_c; i++) {
                // Add name of Literal
                GenericLiteral* literal = formula->all_literals_v[i];
                char* lit_str = literal->name;
                StringBuilder_append_string(builder, lit_str);
                
                // Add separator char
                if (i != formula->all_literals_c - 1) {
                    StringBuilder_append_char(builder, ',');
                }
            }
            
            char* str = StringBuilder_destroy_to_string(builder);
            printf("  %s)\n", str);
            free(str);
        }
        
        // Print all unset Literals
        {
            // Create string
            StringBuilder* builder = StringBuilder_create(100);
            StringBuilder_append_string(builder, "Available literals: (");
            
            int i = 0;
            for (LinkedListNode* iter = unset_literals->head; iter != NULL; iter = iter->next) {
                // Add name of Literal
                GenericLiteral* literal = iter->data;
                char* lit_str = literal->name;
                StringBuilder_append_string(builder, lit_str);
                
                // Add separator char
                if (i != unset_literals->size - 1) {
                    StringBuilder_append_char(builder, ',');
                }
                
                i++;
            }
            
            char* str = StringBuilder_destroy_to_string(builder);
            printf("  %s)\n", str);
            free(str);
        }
        
        // Free list of unset Literals
        LinkedList_destroy(unset_literals, false);
    }
    #endif
    
    // Just return the first available (unset) Literal
    for (int i = 0; i < formula->all_literals_c; i++) {
        GenericLiteral* lit = formula->all_literals_v[i];
        
        if (GenericLiteral_get_assignment(lit) == LiteralAssignment_UNSET) {
            #if VERBOSE_DPLL
                printf("  Picked unassigned literal: %s\n", lit->name);
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
    // Evaluate current assignment
    #if VERBOSE_DPLL
        printf("  evaluating ...\n");
    #endif
    
    bool all_clauses_assigned = true;
    
    for (LinkedListNode* iter = formula->clauses->head; iter != NULL; iter = iter->next) {
        Clause* clause = iter->data;
        ClauseStatus status = Clause_evaluate(clause);
        
        switch(status) {
            case ClauseStatus_TRUE:
                #if VERBOSE_DPLL
                {
                    char* clause_str = Clause_to_string(clause, CONSTANTS_COLOR_ENABLED);
                    printf("  Clause %s is true ...\n", clause_str);
                    free(clause_str);
                }
                #endif
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
                return FormulaStatus_FALSE;
                break;
            case ClauseStatus_UNDECIDED:
                all_clauses_assigned = false;
                break;
        }
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
    if (all_clauses_assigned) {
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
            
            return true;
        }
        
        // If assignment wasn't correct set the Literal to the
        // non-preferred assignment and rerun.
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


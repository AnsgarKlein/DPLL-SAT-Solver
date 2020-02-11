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


#include "Parser-CNF.h"
#include "Constants.h"
#include "StringBuilder.h"

#include <assert.h>
#include <string.h>


Formula* CNFParser_parse_formula(char* formula_str) {
    assert(formula_str != NULL);
    
    // Create list of Clauses
    LinkedList* clause_list = LinkedList_create((void (*)(void*))&Clause_destroy);
    
    // Create list of ALL Literals
    LinkedList* all_literals = LinkedList_create((void (*)(void*))&GenericLiteral_destroy);
    
    // Search for starting point of Clause
    for (unsigned int i = 0; i < strlen(formula_str); i++) {
        char c = formula_str[i];
        if (c == CONSTANTS_CNFPARSE_CLAUSE_START) {
            // Create string containing the Clause
            StringBuilder* clause_str_builder = StringBuilder_create(30);
            
            // Search end of Clause and add everything between
            // start and end to the Clause-String
            unsigned int p;
            for (p = i+1; p < strlen(formula_str); p++) {
                if (formula_str[p] != CONSTANTS_CNFPARSE_CLAUSE_END) {
                    StringBuilder_append_char(clause_str_builder, formula_str[p]);
                } else {
                    break;
                }
            }
            i = p;
            
            // Create new Clause from string
            char* clause_str = StringBuilder_destroy_to_string(clause_str_builder);
            Clause* new_clause = CNFParser_parse_clause(clause_str, all_literals);
            free(clause_str);
            
            // Add new Clause to list
            LinkedList_append(clause_list, new_clause);
        }
        
    }
    
    // Convert list of all literals to array
    int all_literals_c = all_literals->size;
    GenericLiteral** all_literals_v = malloc(all_literals_c * sizeof(GenericLiteral*));
    {
        int i = 0;
        for (LinkedListNode* iter = all_literals->head; iter != NULL; iter = iter->next) {
            GenericLiteral* lit = iter->data;
            
            all_literals_v[i] = lit;
            
            i++;
        }
    }
    LinkedList_destroy(all_literals, false);
    
    // Create Formula from list of Clauses and array of all Literals
    Formula* formula = Formula_create(clause_list, all_literals_v, all_literals_c);
    
    return formula;
}

Clause* CNFParser_parse_clause(char* clause_str, LinkedList* all_literals) {
    assert(clause_str != NULL);
    assert(all_literals != NULL);
    
    // Create list of all Literals contained in this Clause
    LinkedList* clause_literals = LinkedList_create((void(*)(void*))Literal_destroy);
    
    // Create a Literal string
    StringBuilder* lit_str_builder = StringBuilder_create(10);
    
    for (unsigned int i = 0; i < strlen(clause_str); i++) {
        char c = clause_str[i];
        
        if (c != CONSTANTS_CNFPARSE_LITERAL_DELIMITER) {
            StringBuilder_append_char(lit_str_builder, c);
        }
        
        if (c == CONSTANTS_CNFPARSE_LITERAL_DELIMITER || i+1 == strlen(clause_str)) {
            // Create new Literal from Literal string
            char* lit_str = StringBuilder_destroy_to_string(lit_str_builder);
            Literal* new_literal = CNFParser_parse_literal(lit_str, all_literals);
            free(lit_str);
            lit_str_builder = StringBuilder_create(10);
            
            // Add new Literal to list
            LinkedList_append(clause_literals, new_literal);
        }
    }
    StringBuilder_destroy(lit_str_builder);
    
    // Create array of Literals from list of Literals
    unsigned int literals_c = clause_literals->size;
    Literal** literals_v = malloc(sizeof(Literal*) * literals_c);
    {
        int i = 0;
        for (LinkedListNode* iter = clause_literals->head; iter != NULL; iter = iter->next) {
            literals_v[i] = iter->data;
            i++;
        }
    }
    LinkedList_destroy(clause_literals, false);
    
    // Create Clause from list of Literals
    Clause* new_clause = Clause_create(literals_v, literals_c);
    return new_clause;
}

Literal* CNFParser_parse_literal(char* literal_str, LinkedList* all_literals) {
    assert(literal_str != NULL);
    assert(all_literals != NULL);
    
    // Create name string
    StringBuilder* name_builder = StringBuilder_create(10);
    
    bool negated = false;
    
    // Look for Literal name and check if it is negated
    for (unsigned int i = 0; i < strlen(literal_str); i++) {
        char c = literal_str[i];
        
        if (c == CONSTANTS_CNFPARSE_NEGATE_CHAR) {
            negated = !negated;
        } else if (c == ' ') {
            // skip
        } else {
            StringBuilder_append_char(name_builder, c);
        }
    }
    char* name = StringBuilder_destroy_to_string(name_builder);
    
    // Create new GenericLiteral
    GenericLiteral* new_literal = GenericLiteral_create(name, LiteralAssignment_UNSET);
    
    // Check if Literal is already contained in list of all Literals
    // If it is take that one, if not create a new one and add it to
    // the list of all Literals.
    bool literal_already_contained = false;
    for (LinkedListNode* iter = all_literals->head; iter != NULL; iter = iter->next) {
        GenericLiteral* found_literal = iter->data;
        
        if (GenericLiteral_equals(found_literal, new_literal)) {
            GenericLiteral_destroy(new_literal);
            
            new_literal = found_literal;
            literal_already_contained = true;
            break;
        }
    }
    
    if (!literal_already_contained) {
        LinkedList_append(all_literals, new_literal);
    }
    
    // Create new Literal
    Literal* literal = Literal_create(new_literal, negated);
    GenericLiteral_increase_occurrences(new_literal);
    
    return literal;
}

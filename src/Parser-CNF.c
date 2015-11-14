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

#include <assert.h>
#include <string.h>


Formula* CNFParser_parse_formula(char* formula_str) {
    assert(formula_str != NULL);
    
    const char CLAUSE_START = '{';
    const char CLAUSE_END = '}';
    
    // Create list of Clauses
    LinkedList* clause_list = LinkedList_create((void (*)(void*))&Clause_destroy);
    
    // Create list of ALL Literals
    LinkedList* all_literals = LinkedList_create((void (*)(void*))&GenericLiteral_destroy);
    
    // Search for starting point of Clause
    for (int i = 0; i < strlen(formula_str); i++) {
        char c = formula_str[i];
        if (c == CLAUSE_START) {
            // Create string containing the Clause
            unsigned int clause_str_l = 10;
            char* clause_str = malloc(clause_str_l * sizeof(char));
            assert(clause_str != NULL);
            memset(clause_str, '\0', 1);
            
            // Search end of Clause and add everything between
            // start and end to the Clause-String
            int p;
            for (p = i+1; p < strlen(formula_str); p++) {
                if (formula_str[p] != CLAUSE_END) {
                    // Resize if necessary and add char
                    while ((strlen(clause_str) + 1 + 1) > clause_str_l) {
                        clause_str_l = clause_str_l * 2;
                        clause_str = realloc(clause_str, clause_str_l);
                        assert(clause_str != NULL);
                    }
                    int len = strlen(clause_str);
                    clause_str[len + 0] = formula_str[p];
                    clause_str[len + 1] = '\0';
                } else {
                    break;
                }
            }
            i = p;
            
            // Shrink Clause string to minimum size required
            clause_str_l = (strlen(clause_str) + 1) * sizeof(char);
            clause_str = realloc(clause_str, clause_str_l);
            assert(clause_str != NULL);
            
            // Create new Clause from string
            Clause* new_clause = CNFParser_parse_clause(clause_str, all_literals);
            free(clause_str);
            
            // Add new Clause to list
            LinkedList_prepend(clause_list, new_clause);
        }
        
    }
    
    // Create Formula from list of Clauses and array of all Literals
    Formula* formula = Formula_create(clause_list, all_literals);
    
    return formula;
}

Clause* CNFParser_parse_clause(char* clause_str, LinkedList* all_literals) {
    assert(clause_str != NULL);
    assert(all_literals != NULL);
    
    const char LITERAL_DELIMITER = ',';
    
    unsigned int clause_literals_size = 5;
    unsigned int clause_literals_filled = 0;
    Literal** clause_literals_v = malloc(clause_literals_size * sizeof(Literal*));
    assert(clause_literals_v != NULL);
    
    // Create a Literal string
    unsigned int lit_str_l = 2;
    char* lit_str = malloc(lit_str_l * sizeof(char));
    assert(lit_str != NULL);
    memset(lit_str, '\0', 1);
    
    for (int i = 0; i < strlen(clause_str); i++) {
        char c = clause_str[i];
        
        if (c != LITERAL_DELIMITER) {
           // Resize if necessary and add char
            while ((strlen(lit_str) + 1 + 1) > lit_str_l) {
                lit_str_l = lit_str_l * 2;
                lit_str = realloc(lit_str, lit_str_l);
                assert(lit_str != NULL);
            }
            int len = strlen(lit_str);
            lit_str[len + 0] = c;
            lit_str[len + 1] = '\0';
        }
        
        if (c == LITERAL_DELIMITER || i+1 == strlen(clause_str)) {
            // Shrink Literal string to minimum size required
            lit_str_l = (strlen(lit_str) + 1) * sizeof(char);
            lit_str = realloc(lit_str, lit_str_l);
            assert(lit_str != NULL);
            
            // Create new Literal from string
            Literal* new_literal = CNFParser_parse_literal(lit_str, all_literals);
            memset(lit_str, '\0', lit_str_l);
            
            // Resize if necessary and add new Literal to array
            while ((clause_literals_filled + 1) > clause_literals_size) {
                clause_literals_size = clause_literals_size * 2;
                clause_literals_v = realloc(clause_literals_v, clause_literals_size * sizeof(Literal*));
                assert(clause_literals_v != NULL);
            }
            clause_literals_v[clause_literals_filled] = new_literal;
            clause_literals_filled++;
        }
    }
    free(lit_str);
    
    // Shrink Literals array to minimum size required
    clause_literals_size = clause_literals_filled;
    clause_literals_v = realloc(clause_literals_v, clause_literals_size * sizeof(Literal*));
    assert(clause_literals_v != NULL);
    
    // Create Clause from array of Literals
    Clause* new_clause = Clause_create(clause_literals_v, clause_literals_size);
    
    return new_clause;
}

Literal* CNFParser_parse_literal(char* literal_str, LinkedList* all_literals) {
    assert(literal_str != NULL);
    assert(all_literals != NULL);
    
    const char NEGATE_CHAR = '-';
    
    unsigned int name_l = 2;
    char* name = malloc(name_l * sizeof(char));
    assert(name != NULL);
    memset(name, '\0', 1);
    
    bool negated = false;
    
    // Look for Literal name
    for (int i = 0; i < strlen(literal_str); i++) {
        char c = literal_str[i];
        if (c == NEGATE_CHAR) {
            negated = !negated;
        } else if (c == ' ') {
            // skip
        } else {
           // Resize if necessary and add char
            while ((strlen(name) + 1 + 1) > name_l) {
                name_l = name_l * 2;
                name = realloc(name, name_l);
                assert(name != NULL);
            }
            int len = strlen(name);
            name[len + 0] = c;
            name[len + 1] = '\0';
        }
    }
    
    // Shrink Literal name string to minimum size required
    name_l = (strlen(name) + 1);
    name = realloc(name, name_l  * sizeof(char));
    assert(name != NULL);
    
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
        LinkedList_prepend(all_literals, new_literal);
    }
    
    GenericLiteral_increase_occurrences(new_literal);
    Literal* literal = Literal_create(new_literal, negated);
    
    return literal;
}

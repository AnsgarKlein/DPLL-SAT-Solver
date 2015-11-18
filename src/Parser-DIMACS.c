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


#include "Parser-DIMACS.h"
#include "StringBuilder.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


Formula* DIMACSParser_parse_formula(char* str) {
    // Split string into array of lines
    unsigned int lines_c = 10;
    char** lines_v = malloc(lines_c * sizeof(char*));
    {
        unsigned int lines_filled = 0;
        
        char* tmp = strtok(str, "\n");
        while (tmp != NULL) {
            // Resize if necessary
            if (lines_filled + 1 > lines_c) {
                lines_c *= 4;
                lines_v = realloc(lines_v, lines_c * sizeof(char*));
            }
            
            // Add to array
            lines_v[lines_filled++] = tmp;
            
            // Split off next line from string
            tmp = strtok(NULL, "\n");
        }
        
        // Shrink array to minimum size required
        lines_c = lines_filled;
        lines_v = realloc(lines_v, lines_c * sizeof(char*));
    }
    
    // Skip first lines with comments
    unsigned int linec = 0;
    for (linec = 0; linec < lines_c; linec++) {
        if (lines_v[linec][0] == 'c') {
            // skip
        } else {
            break;
        }
    }
    
    unsigned int literals_count = 0;
    unsigned int clauses_count = 0;
    
    // Parse problem line
    {
        bool problem_line_error = false;
        
        // Check if problem line has exactly these values separated by whitespaces:
        // "p"
        // "cnf"
        // <Number of Literals>
        // <Number of Clauses>
        int i = 0;
        char* tmp = strtok(lines_v[linec], " ");
        while (tmp != NULL) {
            if (i == 0 && !strcmp(tmp, "p") == 0) {
                problem_line_error = true;
            } else if (i == 1 && !strcmp(tmp, "cnf") == 0) {
                problem_line_error = true;
            } else if (i == 2) {
                literals_count = strtol(tmp, NULL, 10);
            } else if (i == 3) {
                clauses_count = strtol(tmp, NULL, 10);
            }
            
            i++;
            tmp = strtok(NULL, " ");
        }
        
        if (i != 4) {
            problem_line_error = true;
        }
        
        if (problem_line_error) {
            fprintf(stderr, "Error - missing/malformed problem line\n");
            free(lines_v);
            return NULL;
        }
        
        if (literals_count == 0) {
            fprintf(stderr, "Error - number of literals is 0 according to problem line!\n");
            fprintf(stderr, " - this can't be right\n");
            free(lines_v);
            return NULL;
        }
        
        if (clauses_count == 0) {
            fprintf(stderr, "Error - number of clauses is 0 according to problem line!\n");
            fprintf(stderr, " - this can't be right\n");
            free(lines_v);
            return NULL;
        }
    }
    linec++;
    
    // Create list of ALL Literals in this Formula
    LinkedList* all_literals = LinkedList_create((void(*)(void*))GenericLiteral_destroy);
    
    // Create list of Clauses in this Formula
    LinkedList* clauses = LinkedList_create((void(*)(void*))Clause_destroy);
    
    // Parse Clauses
    unsigned int clauses_found = 0;
    
    for (; linec < lines_c; linec++) {
        // Ignore it if the last line is empty
        if (linec == lines_c - 1) {
            if (strcmp(lines_v[linec],"") == 0) {
                continue;
            }
        }
        
        Clause* clause = DIMACSParser_parse_clause(lines_v[linec], all_literals);
        if (clause == NULL) {
            free(lines_v);
            LinkedList_destroy(all_literals, true);
            LinkedList_destroy(clauses, true);
            return NULL;
        }
        
        LinkedList_append(clauses, clause);
        clauses_found++;
    }
    free(lines_v);
    
    // Check if given number of Clauses was correct
    if (clauses_count != clauses_found) {
        fprintf(stderr, "Error - number of clauses given in problem line is not correct!\n");
        LinkedList_destroy(all_literals, true);
        LinkedList_destroy(clauses, true);
        return NULL;
    }
    
    // Check if given number of Literals was correct
    if (literals_count != all_literals->size) {
        fprintf(stderr, "Error - number of literals given in problem line is not correct!\n");
        LinkedList_destroy(all_literals, true);
        LinkedList_destroy(clauses, true);
        return NULL;
    }
    
    return Formula_create(clauses, all_literals);
}

Clause* DIMACSParser_parse_clause(char* line, LinkedList* all_literals) {
    // Empty lines are not permitted
    if (strcmp(line, "") == 0) {
        fprintf(stderr, "Error - empty lines are not permitted!\n");
        return NULL;
    }
    
    // Split line by space
    unsigned int line_c = 30;
    char** line_v = malloc(line_c * sizeof(char*));
    {
        unsigned int line_filled = 0;
        
        char* tmp = strtok(line, " ");
        while (tmp != NULL) {
            // Resize if necessary
            if (line_filled + 1 > line_c) {
                line_c *= 4;
                line_v = realloc(line_v, line_c * sizeof(char*));
            }
            
            // Add to array
            line_v[line_filled++] = tmp;
            
            // Split off next line from string
            tmp = strtok(NULL, " ");
        }
        
        // Shrink array to minimum size required
        line_c = line_filled;
        line_v = realloc(line_v, line_c * sizeof(char*));
    }
    
    // Create list of Literals in Clause
    LinkedList* clause_literals = LinkedList_create((void(*)(void*))Literal_destroy);
    
    // Parse each Literal separately
    for (int i = 0; i < line_c; i++) {
        // Parse Literal int (name)
        signed int a = strtol(line_v[i], NULL, 10);
        
        // The last element of each line has to be 0
        if (i == line_c - 1) {
            if (a != 0) {
                fprintf(stderr, "Error - Last element of line was not 0!\n");
                return NULL;
            }
            break;
        }
        
        // 0 is only allowed to be the last element of each line.
        if (a == 0) {
            fprintf(stderr, "Error - Found Literal 0, which is not permitted!\n");
            return NULL;
        }
        
        // Parse Literal
        bool negated = false;
        char* name = malloc(sizeof(int) * sizeof(char) + 1 * sizeof(char));
        
        if (a < 0) {
            negated = true;
            a = abs(a);
        }
        snprintf(name, 33, "%d", a);
        name = realloc(name, (strlen(name) + 1) * sizeof(char));
        
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
        
        // Add Literal to list of Literals
        LinkedList_append(clause_literals, literal);
    }
    free(line_v);
    
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
    
    return Clause_create(literals_v, literals_c);
}

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


#include "Main.h"
#include "Parser-CNF.h"
#include "Parser-DIMACS.h"
#include "Formula.h"
#include "Constants.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>


int main(int argc, char* argv[]) {
    // Print help and exit if started with "--help" or "-h"
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printf("Usage:\n");
            printf("  %s [OPTIONS...] < FILE\n", argv[0]);
            printf("\n");
            printf("\n");
            printf("Options:\n");
            printf("  -h --help\t\tPrint this help\n");
            printf("  -c --cnf\t\tIndicate that the given formula is in CNF format.\n");
            printf("  \t\t\tYou can customize this format using:\n");
            printf("    --cstart CHAR\tuse CHAR as starting character of a clause\n");
            printf("  \t\t\tin a formula in CNF form.\n");
            printf("    --cend CHAR\t\tuse CHAR as ending character of a clause\n");
            printf("  \t\t\tin a formula in CNF form.\n");
            printf("    --cdel CHAR\t\tuse CHAR as delimiting character inside clauses\n");
            printf("  \t\t\tof a formula in CNF form.\n");
            printf("    --cneg CHAR\t\tuse CHAR as negating character in front of literals\n");
            printf("  \t\t\tinside clauses in a formula in CNF form.\n");
            printf("  -d --dimacs\t\tIndicate that the given formula is in DIMACS format.\n");
            printf("\n");
            printf("\n");
            printf("Examples:\n");
            printf("  echo \"{A, B}, {-A, -B}, {-C}, {E, D}\" | %s --cnf\n", argv[0]);
            printf("  \t\t\tIf you don't give detailed options '{’ and ’}’ are\n");
            printf("  \t\t\tthe default characters for starting and ending a\n");
            printf("  \t\t\tclause.\n");
            printf("  \t\t\t',' delimits literals and '-' is used for negating.\n");
            printf("\n");
            printf("  echo \"{A, B}; {-A, -B}  {-C}{E, D}\" | %s --cnf\n", argv[0]);
            printf("  \t\t\tEverything between clauses is ignored.\n");
            printf("\n");
            printf("  echo \"(A v B) ^ (-A v -B) ^ (-C) ^ (E v D)\" | \\\n");
            printf("  %s --cnf --cstart \"(\" --cend \")\" --cdel \"v\"\n", argv[0]);
            printf("  \t\t\tYou can easily customize the details of the CNF form\n");
            printf("  \t\t\tif your formula is not represented as a set but as\n");
            printf("  \t\t\tas a conjunction of disjunctions.\n");
            return 0;
        }
    }
    
    // Check if inputs are sane
    
    // Check if all prerequisites for inputs are present
    {
        char* optionals[] = {"--cstart", "--cend", "--cdel", "--cneg"};
        char* optional = NULL;
        bool optional_used = false;
        
        char* prerequisites[] = {"-c", "--cnf"};
        bool prerequisites_present = false;
        
        // Check every parameter
        for (int i = 0; i < argc; i++) {
            
            // Check if optional parameters are used
            for (int p = 0; p < 4; p++) {
                if (strcmp(argv[i], optionals[p]) == 0) {
                    optional = optionals[p];
                    optional_used = true;
                    break;
                }
            }
            
            // Check for prerequisites
            for (int p = 0; p < 2; p++) {
                if (strcmp(argv[i], prerequisites[p]) == 0) {
                    prerequisites_present = true;
                    break;
                }
            }
        }
        
        // If prerequisites are not present abort.
        if (optional_used && !prerequisites_present) {
            fprintf(stderr, "%s needs --cnf\n", optional);
            return 1;
        }
    }
    
    // Check if format of Formula is unambiguously
    {
        char* cnfs[] = {"-c", "--cnf"};
        bool cnf_declared = false;
        
        char* dimacs[] = {"-d", "--dimacs"};
        bool dimacs_declared = false;
        
        for (int i = 0; i < argc; i++) {
            for (int p = 0; p < 2; p++) {
                if (strcmp(argv[i], cnfs[p]) == 0) {
                    cnf_declared = true;
                }
                
                if (strcmp(argv[i], dimacs[p]) == 0) {
                    dimacs_declared = true;
                }
            }
        }
        
        if (cnf_declared && dimacs_declared) {
            fprintf(stderr, "Specify format of formula unambiguously\n");
            return 1;
        }
    }
    
    // Apply options
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--cstart") == 0) {
            if (argc == i + 1) {
                fprintf(stderr, "%s needs a parameter!\n", argv[i]);
                return 1;
            }
            
            if (strlen(argv[i+1]) > 1) {
                fprintf(stderr, "%s parameter can only be one character!\n", argv[i]);
                return 1;
            }
            
            CONSTANTS_CNFPARSE_CLAUSE_START = argv[i+1][0];
        } else if (strcmp(argv[i], "--cend") == 0) {
            if (argc == i + 1) {
                fprintf(stderr, "%s needs a parameter!\n", argv[i]);
                return 1;
            }
            
            if (strlen(argv[i+1]) > 1) {
                fprintf(stderr, "%s parameter can only be one character!\n", argv[i]);
                return 1;
            }
            
            CONSTANTS_CNFPARSE_CLAUSE_END = argv[i+1][0];
        } else if (strcmp(argv[i], "--cdel") == 0) {
            if (argc == i + 1) {
                fprintf(stderr, "%s needs a parameter!\n", argv[i]);
                return 1;
            }
            
            if (strlen(argv[i+1]) > 1) {
                fprintf(stderr, "%s parameter can only be one character!\n", argv[i]);
                return 1;
            }
            
            CONSTANTS_CNFPARSE_LITERAL_DELIMITER = argv[i+1][0];
        } else if (strcmp(argv[i], "--cneg") == 0) {
            if (argc == i + 1) {
                fprintf(stderr, "%s needs a parameter!\n", argv[i]);
                return 1;
            }
            
            if (strlen(argv[i+1]) > 1) {
                fprintf(stderr, "%s parameter can only be one character!\n", argv[i]);
                return 1;
            }
            
            CONSTANTS_CNFPARSE_NEGATE_CHAR = argv[i+1][0];
        }
    }
    
    // Read formula from stdin
    char* formula_str = NULL;
    {
        size_t size = 0;
        size_t len = 0;
        size_t last = 0;
        
        do {
            size += BUFSIZ;
            
            formula_str = realloc(formula_str, size);
            assert(formula_str != NULL);
            
            fgets(formula_str + last, size, stdin);
            len = strlen(formula_str);
            last = len - 1;
        } while (!feof(stdin) && formula_str[last] != '\n');
    }
    
    // Decide whether formula is in dimacs format or in cnf format.
    Formula* formula = NULL;
    {
        // If format is set on command line we'll use that
        bool cnf_format = false;
        bool dimacs_format = false;
        
        {
            char* cnfs[] = {"-c", "--cnf"};
            char* dimacs[] = {"-d", "--dimacs"};
            
            for (int i = 0; i < argc; i++) {
                for (int p = 0; p < 2; p++) {
                    if (strcmp(argv[i], cnfs[p]) == 0) {
                        cnf_format = true;
                    }
                    
                    if (strcmp(argv[i], dimacs[p]) == 0) {
                        dimacs_format = true;
                    }
                }
            }
        }
        
        // If format is not set we'll try to guess it.
        if (!dimacs_format && !cnf_format) {
            for (int i = 0; i < strlen(formula_str); i++) {
                if (formula_str[i] == CONSTANTS_CNFPARSE_CLAUSE_START) {
                    cnf_format = true;
                }
            }
        }
        
        if (cnf_format) {
            formula = CNFParser_parse_formula(formula_str);
        } else {
            formula = DIMACSParser_parse_formula(formula_str);
        }
    }
    free(formula_str);
    if (formula == NULL) {
        return 1;
    }
    
    // Run DPLL
    bool satisfiable = Formula_dpll(formula);
    if (!satisfiable) {
        printf("Formula is not satisfiable\n");
    } else {
        char* str = Formula_to_assignment_string(formula, true, CONSTANTS_COLOR_ENABLED);
        printf("%s\n", str);
        free(str);
    }
    
    Formula_destroy(formula);
    return 0;
}

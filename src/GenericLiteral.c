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


#include "Literal.h"
#include "Constants.h"

#include <assert.h>
#include <string.h>


GenericLiteral* GenericLiteral_create(char* name, LiteralAssignment assignment) {
    assert(name != NULL);
    
    GenericLiteral* literal = malloc(sizeof(Literal));
    assert(literal != NULL);
    
    literal->name = name;
    literal->assignment = assignment;
    literal->occurrences = 0;
    
    return literal;
};

void GenericLiteral_destroy(GenericLiteral* literal) {
    assert(literal != NULL);
    
    // Free Literal name
    free(literal->name);
    
    // Free GenericLiteral
    free(literal);
};

void GenericLiteral_increase_occurrences(GenericLiteral* literal) {
    assert(literal != NULL);
    
    literal->occurrences++;
}

int GenericLiteral_get_occurrences(GenericLiteral* literal) {
    assert(literal != NULL);
    
    return literal->occurrences;
}

char* GenericLiteral_get_name(GenericLiteral* literal) {
    assert(literal != NULL);
    
    return literal->name;
}

void GenericLiteral_assign(GenericLiteral* literal, bool b) {
    assert(literal != NULL);
    
    literal->assignment = b ? LiteralAssignment_TRUE : LiteralAssignment_FALSE;
}

void GenericLiteral_unassign(GenericLiteral* literal) {
    assert(literal != NULL);
    
    literal->assignment = LiteralAssignment_UNSET;
}

LiteralAssignment GenericLiteral_get_assignment(GenericLiteral* literal) {
    assert(literal != NULL);
    
    return literal->assignment;
}

bool GenericLiteral_equals(GenericLiteral* literal1, GenericLiteral* literal2) {
    assert(literal1 != NULL);
    assert(literal2 != NULL);
    
    if (literal1 == literal2) {
        return true;
    }
    
    char* name1 = GenericLiteral_get_name(literal1);
    char* name2 = GenericLiteral_get_name(literal2);
    if (strcmp(name1, name2) == 0) {
        return true;
    }
    
    return false;
}

char* GenericLiteral_to_string(GenericLiteral* literal, bool color) {
    assert(literal != NULL);
    
    uint buf_l = 10;
    char* buf = malloc(buf_l * sizeof(char));
    assert(buf != NULL);
    memset(buf, '\0', 1);
    
    // If color is wanted add the correct color code to buffer
    if (color) {
        switch(literal->assignment) {
        case LiteralAssignment_TRUE:
            // Resize if necessary and add string
            while ((strlen(buf) + strlen(CONSTANTS_COLOR_PREFIX_TRUE) + 1) > buf_l) {
                buf_l = buf_l * 2;
                buf = realloc(buf, buf_l);
            }
            strcat(buf, CONSTANTS_COLOR_PREFIX_TRUE);
            break;
        case LiteralAssignment_FALSE:
            // Resize if necessary and add string
            while ((strlen(buf) + strlen(CONSTANTS_COLOR_PREFIX_FALSE) + 1) > buf_l) {
                buf_l = buf_l * 2;
                buf = realloc(buf, buf_l);
            }
            strcat(buf, CONSTANTS_COLOR_PREFIX_FALSE);
            break;
        case LiteralAssignment_UNSET:
            break;
        default:
            assert(0);
        }
    }
    
    //Add name to buffer
    {
        // Resize if necessary and add string
        while ((strlen(buf) + strlen(literal->name) + 1) > buf_l) {
            buf_l = buf_l * 2;
            buf = realloc(buf, buf_l);
        }
        strcat(buf, literal->name);
    }
    
    // If color is wanted add the default color code after the name
    if (color) {
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

char* GenericLiteral_to_assignment_string(GenericLiteral* literal, bool color) {
    assert(literal != NULL);
    
    uint buf_l = 10;
    char* buf = malloc(buf_l * sizeof(char));
    assert(buf != NULL);
    memset(buf, '\0', 1);
    
    {
    uint size_needed = 0;
    
        switch(literal->assignment) {
        case LiteralAssignment_TRUE:
            size_needed += strlen(literal->name);
            size_needed += strlen("=true");
            
            if (color) {
                size_needed += strlen(CONSTANTS_COLOR_PREFIX_TRUE);
                size_needed += strlen(CONSTANTS_COLOR_SUFFIX);
                
                while (size_needed + 1 > buf_l) {
                    buf_l = buf_l * 2;
                    buf = realloc(buf, buf_l);
                }
                
                strcat(buf, CONSTANTS_COLOR_PREFIX_TRUE);
                strcat(buf, literal->name);
                strcat(buf, "=true");
                strcat(buf, CONSTANTS_COLOR_SUFFIX);
            } else {
                while (size_needed + 1 > buf_l) {
                    buf_l = buf_l * 2;
                    buf = realloc(buf, buf_l);
                }
                
                strcat(buf, literal->name);
                strcat(buf, "=true");
            }
            break;
        case LiteralAssignment_FALSE:
            size_needed += strlen(literal->name);
            size_needed += strlen("=false");
            
            if (color) {
                size_needed += strlen(CONSTANTS_COLOR_PREFIX_FALSE);
                size_needed += strlen(CONSTANTS_COLOR_SUFFIX);
                
                while (size_needed + 1 > buf_l) {
                    buf_l = buf_l * 2;
                    buf = realloc(buf, buf_l);
                }
                
                strcat(buf, CONSTANTS_COLOR_PREFIX_FALSE);
                strcat(buf, literal->name);
                strcat(buf, "=false");
                strcat(buf, CONSTANTS_COLOR_SUFFIX);
            } else {
                while (size_needed + 1 > buf_l) {
                    buf_l = buf_l * 2;
                    buf = realloc(buf, buf_l);
                }
                
                strcat(buf, literal->name);
                strcat(buf, "=false");
            }
            break;
        case LiteralAssignment_UNSET:
            size_needed += strlen(literal->name);
            size_needed += strlen("=?");
            
            while (size_needed + 1 > buf_l) {
                buf_l = buf_l * 2;
                buf = realloc(buf, buf_l);
            }
            
            strcat(buf, literal->name);
            strcat(buf, "=?");
        
            break;
        default:
            assert(0);
        }
    }
    
    // Shrink buffer to minimum required
    buf_l = (strlen(buf) + 1) * sizeof(char);
    buf = realloc(buf, buf_l);
    
    return buf;
}

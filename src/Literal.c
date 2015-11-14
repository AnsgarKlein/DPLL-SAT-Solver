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


Literal* Literal_create(GenericLiteral* generic_literal, bool negated) {
    assert(generic_literal != NULL);
    
    Literal* literal = malloc(sizeof(Literal));
    assert(literal != NULL);
    
    literal->generic_literal = generic_literal;
    literal->negated = negated;
    
    return literal;
}

void Literal_destroy(Literal* literal) {
    assert(literal != NULL);
    
    // Don't free GenericLiteral!
    
    // Free Literal
    free(literal);
}

Literal* Literal_clone(Literal* literal) {
    Literal* cloned_literal = NULL;
    if (literal->negated) {
        cloned_literal = Literal_create(literal->generic_literal, true);
    } else {
        cloned_literal = Literal_create(literal->generic_literal, false);
    }
    
    return cloned_literal;
}

char* Literal_to_string(Literal* literal, bool color) {
    assert(literal != NULL);
    
    uint buf_l = 10;
    char* buf = malloc(buf_l * sizeof(char));
    assert(buf != NULL);
    memset(buf, '\0', 1);
    
    // If color is wanted add the correct color code to buffer
    if (color) {
        switch(literal->generic_literal->assignment) {
        case LiteralAssignment_TRUE:
            if (literal->negated) {
                // Resize if necessary and add string
                while ((strlen(buf) + strlen(CONSTANTS_COLOR_PREFIX_FALSE) + 1) > buf_l) {
                    buf_l = buf_l * 2;
                    buf = realloc(buf, buf_l);
                }
                strcat(buf, CONSTANTS_COLOR_PREFIX_FALSE);
            } else {
                // Resize if necessary and add string
                while ((strlen(buf) + strlen(CONSTANTS_COLOR_PREFIX_TRUE) + 1) > buf_l) {
                    buf_l = buf_l * 2;
                    buf = realloc(buf, buf_l);
                }
                strcat(buf, CONSTANTS_COLOR_PREFIX_TRUE);
            }
            break;
        case LiteralAssignment_FALSE:
            if (literal->negated) {
                // Resize if necessary and add string
                while ((strlen(buf) + strlen(CONSTANTS_COLOR_PREFIX_TRUE) + 1) > buf_l) {
                    buf_l = buf_l * 2;
                    buf = realloc(buf, buf_l);
                }
                strcat(buf, CONSTANTS_COLOR_PREFIX_TRUE);
            } else {
                // Resize if necessary and add string
                while ((strlen(buf) + strlen(CONSTANTS_COLOR_PREFIX_FALSE) + 1) > buf_l) {
                    buf_l = buf_l * 2;
                    buf = realloc(buf, buf_l);
                }
                strcat(buf, CONSTANTS_COLOR_PREFIX_FALSE);
            }
            break;
        case LiteralAssignment_UNSET:
            break;
        default:
            assert(0);
        }
    }
    
    // If Literal is negated add the negated character to buffer
    if (literal->negated == true) {
        // Resize if necessary and add string
        while ((strlen(buf) + strlen(CONSTANTS_NEGATE_CHAR) + 1) > buf_l) {
            buf_l = buf_l * 2;
            buf = realloc(buf, buf_l);
        }
        strcat(buf, CONSTANTS_NEGATE_CHAR);
    }
    
    //Add name to buffer
    {
        // Resize if necessary and add string
        while ((strlen(buf) + strlen(literal->generic_literal->name) + 1) > buf_l) {
            buf_l = buf_l * 2;
            buf = realloc(buf, buf_l);
        }
        strcat(buf, literal->generic_literal->name);
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







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
#include "StringBuilder.h"

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
    
    // Create string
    StringBuilder* builder = StringBuilder_create(10);
    
    // If color is wanted add the correct color code to string
    if (color) {
        switch(literal->generic_literal->assignment) {
        case LiteralAssignment_TRUE:
            if (literal->negated) {
                StringBuilder_append_string(builder, CONSTANTS_COLOR_PREFIX_FALSE);
            } else {
                StringBuilder_append_string(builder, CONSTANTS_COLOR_PREFIX_TRUE);
            }
            break;
        case LiteralAssignment_FALSE:
            if (literal->negated) {
                StringBuilder_append_string(builder, CONSTANTS_COLOR_PREFIX_TRUE);
            } else {
                StringBuilder_append_string(builder, CONSTANTS_COLOR_PREFIX_FALSE);
            }
            break;
        case LiteralAssignment_UNSET:
            break;
        default:
            assert(0);
        }
    }
    
    // If Literal is negated add the negated character to string
    if (literal->negated == true) {
        StringBuilder_append_string(builder, CONSTANTS_NEGATE_CHAR);
    }
    
    //Add name to string
    StringBuilder_append_string(builder, literal->generic_literal->name);
    
    // If color is wanted add the default color code after the name
    if (color) {
        StringBuilder_append_string(builder, CONSTANTS_COLOR_SUFFIX);
    }
    
    return StringBuilder_destroy_to_string(builder);
}

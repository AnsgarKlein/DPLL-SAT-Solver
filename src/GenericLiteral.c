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


GenericLiteral* GenericLiteral_create(char* name, LiteralAssignment assignment) {
    assert(name != NULL);

    GenericLiteral* literal = malloc(sizeof(Literal));
    assert(literal != NULL);

    literal->name = name;
    literal->assignment = assignment;
    literal->occurrences = 0;

    return literal;
}

void GenericLiteral_destroy(GenericLiteral* literal) {
    assert(literal != NULL);

    // Free Literal name
    free(literal->name);

    // Free GenericLiteral
    free(literal);
}

void GenericLiteral_increase_occurrences(GenericLiteral* literal) {
    assert(literal != NULL);

    literal->occurrences++;
}

int GenericLiteral_get_occurrences(GenericLiteral* literal) {
    assert(literal != NULL);

    return literal->occurrences;
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

    char* name1 = literal1->name;
    char* name2 = literal2->name;
    if (strcmp(name1, name2) == 0) {
        return true;
    }

    return false;
}

char* GenericLiteral_to_string(GenericLiteral* literal, bool color) {
    assert(literal != NULL);

    // Create string
    StringBuilder* builder = StringBuilder_create(10);

    // If color is wanted add the correct color code to string
    if (color) {
        switch(literal->assignment) {
        case LiteralAssignment_TRUE:
            StringBuilder_append_string(builder, CONSTANTS_COLOR_PREFIX_TRUE);
            break;
        case LiteralAssignment_FALSE:
            StringBuilder_append_string(builder, CONSTANTS_COLOR_PREFIX_FALSE);
            break;
        case LiteralAssignment_UNSET:
            break;
        default:
            assert(0);
        }
    }

    //Add name to string
    StringBuilder_append_string(builder, literal->name);

    // If color is wanted add the default color code after the name
    if (color) {
        StringBuilder_append_string(builder, CONSTANTS_COLOR_SUFFIX);
    }

    return StringBuilder_destroy_to_string(builder);
}

char* GenericLiteral_to_assignment_string(GenericLiteral* literal, bool color) {
    assert(literal != NULL);

    // Create string
    StringBuilder* builder = StringBuilder_create(10);

    {
        switch(literal->assignment) {
        case LiteralAssignment_TRUE:
            if (color) {
                StringBuilder_append_string(builder, CONSTANTS_COLOR_PREFIX_TRUE);
                StringBuilder_append_string(builder, literal->name);
                StringBuilder_append_string(builder, "=true");
                StringBuilder_append_string(builder, CONSTANTS_COLOR_SUFFIX);
            } else {
                StringBuilder_append_string(builder, literal->name);
                StringBuilder_append_string(builder, "=true");
            }
            break;
        case LiteralAssignment_FALSE:
            if (color) {
                StringBuilder_append_string(builder, CONSTANTS_COLOR_PREFIX_FALSE);
                StringBuilder_append_string(builder, literal->name);
                StringBuilder_append_string(builder, "=false");
                StringBuilder_append_string(builder, CONSTANTS_COLOR_SUFFIX);
            } else {
                StringBuilder_append_string(builder, literal->name);
                StringBuilder_append_string(builder, "=false");
            }
            break;
        case LiteralAssignment_UNSET:
            StringBuilder_append_string(builder, literal->name);
            StringBuilder_append_string(builder, "=?");
            break;
        default:
            assert(0);
        }
    }

    return StringBuilder_destroy_to_string(builder);
}

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

#include <assert.h>


LiteralAssignmentArray* LiteralAssignmentArray_create(GenericLiteral** literals, bool* assignments, unsigned int size) {
    LiteralAssignmentArray* assignment_array;
    assignment_array = malloc(sizeof(LiteralAssignmentArray));
    assert(assignment_array != NULL);
    
    assignment_array->literals = literals;
    
    assignment_array->assignments = assignments;
    assignment_array->size = size;
    
    return assignment_array;
}

void LiteralAssignmentArray_destroy(LiteralAssignmentArray* assignment_array) {
    // Free Literal array
    free(assignment_array->literals);
    
    // Free bool array
    free(assignment_array->assignments);
    
    free(assignment_array);
}

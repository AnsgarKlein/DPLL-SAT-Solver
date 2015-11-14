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


#ifndef LITERAL_H
#define LITERAL_H


#include <stdlib.h>
#include <stdbool.h>


/**
 * Represents the possible assignments of a Literal.
 * 
 * TRUE - Literal is true
 * FALSE - Literal is false
 * UNSET - Literal has no assignment
**/
typedef enum {
    LiteralAssignment_TRUE,
    LiteralAssignment_FALSE,
    LiteralAssignment_UNSET
} LiteralAssignment;

/**
 * Represents a Literal that can have an assignment.
**/
typedef struct {
	char* name;
	LiteralAssignment assignment;
	int occurrences;
} GenericLiteral;

/**
 * Wrapper class for GenericLiteral, which adds the possibility of the
 * contained GenericLiteral being negated.
**/
typedef struct {
    GenericLiteral* generic_literal;
    bool negated;
} Literal;

typedef struct {
    GenericLiteral** literals;
    bool* assignments;
    unsigned int size;
} LiteralAssignmentArray;


LiteralAssignmentArray* LiteralAssignmentArray_create(GenericLiteral** literals, bool* assignments, unsigned int size);

void LiteralAssignmentArray_destroy(LiteralAssignmentArray* assignment_array);

/**
 * Creates a new GenericLiteral.
**/
GenericLiteral* GenericLiteral_create(char* name, LiteralAssignment assignment);

/**
 * Destroys a GenericLiteral.
**/
void GenericLiteral_destroy(GenericLiteral* literal);

/**
 * Increases the number of Clauses a Literal occurs in by one.
**/
void GenericLiteral_increase_occurrences(GenericLiteral* literal);

/**
 * Returns the number of Clauses a Literal occurs in.
**/
int GenericLiteral_get_occurrences(GenericLiteral* literal);

/**
 * Assign Literal the given boolean value.
**/
void GenericLiteral_assign(GenericLiteral* literal, bool b);

/**
 * Remove assignment for Literal.
**/
void GenericLiteral_unassign(GenericLiteral* literal);

/**
 * Return assignment for Literal.
**/
LiteralAssignment GenericLiteral_get_assignment(GenericLiteral* literal);

/**
 * Returns whether two Literals are equal.
 * 
 * Note: Two Literals are equal, exactly if their names are equal.
 *       It does NOT matter whether they have the same assignment or not!
**/
bool GenericLiteral_equals(GenericLiteral* literal1, GenericLiteral* literal2);

/**
 * Represent Literal as a string.
**/
char* GenericLiteral_to_string(GenericLiteral* literal, bool color);

/**
 * Represent Literal with its assignment as a string.
**/
char* GenericLiteral_to_assignment_string(GenericLiteral* literal, bool color);

/**
 * Creates a new Literal.
**/
Literal* Literal_create(GenericLiteral* generic_literal, bool negated);

/**
 * Destroys a Literal.
 * 
 * This does not free the GenericLiteral contained!
**/
void Literal_destroy(Literal* literal);

Literal* Literal_clone(Literal* literal);

/**
 * Represent Literal as a string.
**/
char* Literal_to_string(Literal* literal, bool color);

#endif


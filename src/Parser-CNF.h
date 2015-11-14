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


#ifndef PARSER_CNF_H
#define PARSER_CNF_H


#include "Literal.h"
#include "Clause.h"
#include "Formula.h"


Formula* CNFParser_parse_formula(char* str);

Clause* CNFParser_parse_clause(char* clause_str, LinkedList* all_literals);

Literal* CNFParser_parse_literal(char* literal_str, LinkedList* all_literals);

#endif

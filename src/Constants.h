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


#ifndef CONSTANTS_H
#define CONSTANTS_H


#include <stdbool.h>


extern char* CONSTANTS_FORMULA_START;
extern char* CONSTANTS_FORMULA_END;
extern char* CONSTANTS_CLAUSE_START;
extern char* CONSTANTS_CLAUSE_END;
extern char* CONSTANTS_CLAUSE_DELIMITER;
extern char* CONSTANTS_LITERAL_DELIMITER;
extern char* CONSTANTS_NEGATE_CHAR;

extern char CONSTANTS_CNFPARSE_CLAUSE_START;
extern char CONSTANTS_CNFPARSE_CLAUSE_END;
extern char CONSTANTS_CNFPARSE_LITERAL_DELIMITER;
extern char CONSTANTS_CNFPARSE_NEGATE_CHAR;

extern bool CONSTANTS_COLOR_ENABLED;

#define CONSTANTS_COLOR_PREFIX_FALSE    "\033[;91m"
#define CONSTANTS_COLOR_PREFIX_TRUE     "\033[;92m"
#define CONSTANTS_COLOR_SUFFIX          "\033[0;49;39m"

#endif

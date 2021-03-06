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


#ifndef STRINGBUILDER_H
#define STRINGBUILDER_H


#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    unsigned int size;
    unsigned int filled;
    char* buf;
} StringBuilder;


StringBuilder* StringBuilder_create(unsigned int buffer_size);

void StringBuilder_destroy(StringBuilder* builder);

char* StringBuilder_destroy_to_string(StringBuilder* builder);

void StringBuilder_append_string(StringBuilder* builder, char* str);

void StringBuilder_append_char(StringBuilder* builder, char str);

#endif

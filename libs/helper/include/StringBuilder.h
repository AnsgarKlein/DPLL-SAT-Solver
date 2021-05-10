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


#include <stdlib.h>


typedef struct {
    size_t size;
    size_t filled;
    char* buf;
} StringBuilder;


StringBuilder* StringBuilder_create(size_t buffer_size);

void StringBuilder_destroy(StringBuilder* builder);

char* StringBuilder_destroy_to_string(StringBuilder* builder);

void StringBuilder_append_string(StringBuilder* builder, char* str);

void StringBuilder_append_char(StringBuilder* builder, char str);

void StringBuilder_append_int(StringBuilder* builder, int integer);

void StringBuilder_append_uint(StringBuilder* builder, unsigned int integer);

void StringBuilder_append_long(StringBuilder* builder, long integer);


#endif

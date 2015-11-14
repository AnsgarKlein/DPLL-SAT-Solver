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


#include "StringBuilder.h"

#include <assert.h>
#include <string.h>


StringBuilder* StringBuilder_create(unsigned int buffer_size) {
    assert(buffer_size > 0);
    
    StringBuilder* builder = malloc(sizeof(StringBuilder));
    assert(builder != NULL);
    
    builder->size = buffer_size;
    builder->filled = 0;
    
    char* buf = malloc(buffer_size * sizeof(char));
    assert(buf != NULL);
    memset(buf, '\0', 1);
    builder->buf = buf;
    
    return builder;
}

void StringBuilder_destroy(StringBuilder* builder) {
    assert(builder != NULL);
    
    free(builder->buf);
    free(builder);
}

char* StringBuilder_destroy_to_string(StringBuilder* builder) {
    assert(builder != NULL);
    
    char* buf = builder->buf;
    
    // Shrink buffer to minimum size required
    unsigned int buf_l = (strlen(buf) + 1) * sizeof(char);
    buf = realloc(buf, buf_l);
    
    // Free StringBuilder and return string
    free(builder);
    return buf;
}

void StringBuilder_append_string(StringBuilder* builder, char* str) {
    assert(builder != NULL);
    assert(str != NULL);
    
    // Resize if necessary
    while ((strlen(builder->buf) + strlen(str) + 1) > builder->size) {
        builder->size = builder->size * 2;
        builder->buf = realloc(builder->buf, builder->size);
    }
    
    // Add string
    strcat(builder->buf, str);
}

void StringBuilder_append_char(StringBuilder* builder, char c) {
    assert(builder != NULL);
    
    // Resize if necessary
    if ((strlen(builder->buf) + 1 + 1) > builder->size) {
        builder->size = builder->size * 2;
        builder->buf = realloc(builder->buf, builder->size);
    }
    
    // Add char
    unsigned int len = strlen(builder->buf);
    builder->buf[len] = c;
    builder->buf[len + 1] = '\0';
}

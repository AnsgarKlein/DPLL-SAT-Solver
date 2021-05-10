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


#include "StringBuilder_private.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


StringBuilder* StringBuilder_create(size_t buffer_size) {
    assert(buffer_size > 0);

    StringBuilder* builder = malloc(sizeof(StringBuilder));
    assert(builder != NULL);

    builder->size = buffer_size;
    builder->filled = 0;

    char* buf = malloc(buffer_size * sizeof(char));
    assert(buf != NULL);
    buf[0] = '\0';
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
    size_t buf_l = strlen(buf) + 1;
    buf = realloc(buf, buf_l);
    assert(buf != NULL);

    // Free StringBuilder and return string
    free(builder);
    return buf;
}

void StringBuilder_append_string(StringBuilder* builder, char* str) {
    assert(builder != NULL);
    assert(str != NULL);

    // Resize buffer if necessary
    {
        const size_t required_size = strlen(builder->buf) + strlen(str) + 1;
        if (required_size > builder->size) {
            size_t new_len = builder->size;
            do {
                new_len *= 2;
            } while (required_size > new_len);
            builder->size = new_len;
            builder->buf = realloc(builder->buf, new_len);
            assert(builder->buf != NULL);
        }
    }

    // Add string
    strcat(builder->buf, str);
}

void StringBuilder_append_char(StringBuilder* builder, char c) {
    assert(builder != NULL);

    // Resize buffer if necessary
    {
        const size_t required_size = strlen(builder->buf) + 1 + 1;
        if (required_size > builder->size) {
            size_t new_len = builder->size;
            do {
                new_len *= 2;
            } while (required_size > new_len);
            builder->size = new_len;
            builder->buf = realloc(builder->buf, new_len);
            assert(builder->buf != NULL);
        }
    }

    // Add char
    size_t len = strlen(builder->buf);
    builder->buf[len] = c;
    builder->buf[len + 1] = '\0';
}

void StringBuilder_append_int(StringBuilder* builder, int integer) {
    assert(builder != NULL);

    // Convert to string
    char buf[64];
    snprintf(buf, 63, "%d", integer);
    StringBuilder_append_string(builder, buf);
}

void StringBuilder_append_uint(StringBuilder* builder, unsigned int integer) {
    assert(builder != NULL);

    // Convert to string
    char buf[64];
    snprintf(buf, 63, "%u", integer);
    StringBuilder_append_string(builder, buf);
}

void StringBuilder_append_long(StringBuilder* builder, long integer) {
    assert(builder != NULL);

    // Convert to string
    char buf[64];
    snprintf(buf, 63, "%ld", integer);
    StringBuilder_append_string(builder, buf);
}

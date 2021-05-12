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


#ifndef LINKEDLIST_H
#define LINKEDLIST_H


#include <stdbool.h>
#include <stdlib.h>


typedef struct LinkedListNode {
    struct LinkedListNode* next;
    void* data;
} LinkedListNode;

typedef struct {
    LinkedListNode* head;
    LinkedListNode* tail;
    void (*free_data_func)(void* data);
    unsigned int size;
} LinkedList;


LinkedList* LinkedList_create(void (*data_free_func)(void*));

void LinkedList_destroy(LinkedList* list, bool destroy_data);

unsigned int LinkedList_get_size(LinkedList* list);

void LinkedList_prepend(LinkedList* list, void* new_data);

void LinkedList_append(LinkedList* list, void* new_data);

bool LinkedList_remove(LinkedList* list, void* data, bool destroy_data);

void LinkedList_copy_data_to_array(LinkedList* list, void** arr_v, unsigned int* arr_c, size_t element_size);

void LinkedList_as_array(LinkedList* list, void*** arr_v, unsigned int* arr_c);

void LinkedList_destroy_to_array(LinkedList* list, void*** arr_v, unsigned int* arr_c);


#endif

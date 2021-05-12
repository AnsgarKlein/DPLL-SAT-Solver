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


#include "LinkedList_private.h"

#include <assert.h>
#include <string.h>


static inline LinkedListNode* LinkedListNode_create(void* data) {
    assert(data != NULL);

    LinkedListNode* node = malloc(sizeof(LinkedListNode));
    assert(node != NULL);

    node->data = data;
    node->next = NULL;

    return node;
}

static inline void LinkedListNode_destroy(LinkedListNode* node, void (*free_data_func)(void*)) {
    assert(node != NULL);

    if (free_data_func != NULL) {
        free_data_func(node->data);
    }

    free(node);
}

LinkedList* LinkedList_create(void (*free_data_func)(void*)) {
    assert(free_data_func != NULL);

    LinkedList* list = malloc(sizeof(LinkedList));
    assert(list != NULL);

    list->head = NULL;
    list->tail = NULL;
    list->free_data_func = free_data_func;
    list->size = 0;

    return list;
}

void LinkedList_destroy(LinkedList* list, bool destroy_data) {
    assert(list != NULL);

    void (*free_func)(void*) = (destroy_data) ? list->free_data_func : NULL;

    // Free all nodes
    LinkedListNode* node = list->head;
    while (node != NULL) {
        LinkedListNode* next = node->next;
        LinkedListNode_destroy(node, free_func);
        node = next;
    }
    list->size = 0;

    // Free list
    free(list);
}

unsigned int LinkedList_get_size(LinkedList* list) {
    assert(list != NULL);
    return list->size;
}

void LinkedList_prepend(LinkedList* list, void* new_data) {
    assert(list != NULL);
    assert(new_data != NULL);

    LinkedListNode* new_node = LinkedListNode_create(new_data);
    if (list->head == NULL) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        new_node->next = list->head;
        list->head = new_node;
    }
    list->size++;
}

void LinkedList_append(LinkedList* list, void* new_data) {
    assert(list != NULL);
    assert(new_data != NULL);

    LinkedListNode* new_node = LinkedListNode_create(new_data);
    if (list->head == NULL) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }
    list->size++;
}

bool LinkedList_remove(LinkedList* list, void* data, bool destroy_data) {
    assert(list != NULL);
    assert(data != NULL);

    // Try to find node with specified data
    LinkedListNode* prev = NULL;
    LinkedListNode* node = list->head;
    LinkedListNode* next = node->next;
    while (node != NULL) {
        next = node->next;
        if (node->data == data) {
            // Found the correct node
            break;
        }

        prev = node;
        node = next;
    }

    if (node == NULL) {
        // Did not find node
        return false;
    }

    // Remove node from list
    if (prev == NULL) {
        list->head = next;
    } else {
        prev->next = next;
    }

    // Destroy node
    void (*free_data_func)(void*) = (destroy_data) ? list->free_data_func : NULL;
    LinkedListNode_destroy(node, free_data_func);

    list->size--;
    return true;
}

void LinkedList_copy_data_to_array(LinkedList* list, void** arr_v, unsigned int* arr_c, size_t element_size) {
    assert(list != NULL);

    *arr_c = list->size;
    *arr_v = malloc(*arr_c * element_size);

    // Copy to array
    LinkedListNode* node = list->head;
    unsigned int i = 0;
    while (node != NULL) {
        void* data = node->data;
        memcpy(((char*)(*arr_v)) + (element_size * i), data, element_size);

        i++;
        node = node->next;
    }
}

void LinkedList_as_array(LinkedList* list, void*** arr_v, unsigned int* arr_c) {
    assert(list != NULL);

    unsigned int size = list->size;
    void** array = malloc(sizeof(void*) * size);
    assert(array != NULL);

    // Copy to array
    LinkedListNode* node = list->head;
    int i = 0;
    while (node != NULL) {
        array[i] = node->data;

        i++;
        node = node->next;
    }

    *arr_v = array;
    *arr_c = size;
}

void LinkedList_destroy_to_array(LinkedList* list, void*** arr_v, unsigned int* arr_c) {
    assert(list != NULL);

    LinkedList_as_array(list, arr_v, arr_c);
    LinkedList_destroy(list, false);
}

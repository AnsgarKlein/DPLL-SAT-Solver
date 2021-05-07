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


static LinkedListNode* LinkedListNode_create(void* data) {
    assert(data != NULL);

    LinkedListNode* node = malloc(sizeof(LinkedListNode));
    assert(node != NULL);

    node->data = data;
    node->next = NULL;

    return node;
}

static void LinkedListNode_destroy(LinkedListNode* node, void (*free_data_func)(void*)) {
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

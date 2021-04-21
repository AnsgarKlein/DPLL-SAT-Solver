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


static LinkedListNode* LinkedListNode_create_node(void* data) {
    assert(data != NULL);
    
    LinkedListNode* node = malloc(sizeof(LinkedListNode));
    assert(node != NULL);
    
    node->data = data;
    node->next = NULL;
    
    return node;
}

static void LinkedListNode_destroy_node(LinkedListNode* node, bool destroy_data, void (*free_data_func)(void*)) {
    assert(node != NULL);
    assert(free_data_func != NULL);
    
    if (destroy_data) {
        free_data_func(node->data);
    }
    
    free(node);
}

static LinkedListNode* LinkedListNode_remove(LinkedListNode* list, void* data, bool* success, bool destroy_data, void (*free_data_func)(void*)) {
    assert(list != NULL);
    assert(data != NULL);
    assert(free_data_func != NULL);
    
    // If this is the node we want to remove
    if (list->data == data) {
        *success = true;
        
        LinkedListNode* next = list->next;
        LinkedListNode_destroy_node(list, destroy_data, free_data_func);
        return next;
    }
    
    // If this is not the node we want to remove
    if (list->next == NULL) {
        return list;
    }
    
    LinkedListNode* next;
    next = LinkedListNode_remove(list->next, data, success, destroy_data, free_data_func);
    list->next = next;
    return list;
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
    
    // Free all nodes
    if (list->head != NULL) {
        LinkedList_destroy_recursively(list->head, destroy_data, list->free_data_func);
    }
    list->size = 0;
    
    // Free list
    free(list);
}

static void LinkedList_destroy_recursively(LinkedListNode* node, bool destroy_data, void (*free_data_func)(void*)) {
    assert(node != NULL);
    assert(free_data_func != NULL);
    
    // Destroy tail
    if (node->next != NULL) {
        LinkedList_destroy_recursively(node->next, destroy_data, free_data_func);
    }
    
    // Destroy node
    LinkedListNode_destroy_node(node, destroy_data, free_data_func);
}

void LinkedList_prepend(LinkedList* list, void* new_data) {
    assert(list != NULL);
    assert(new_data != NULL);
    
    LinkedListNode* new_node = LinkedListNode_create_node(new_data);
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
    
    LinkedListNode* new_node = LinkedListNode_create_node(new_data);
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
    
    // Try to remove node with specified data
    bool success = false;
    list->head = LinkedListNode_remove(list->head, data, &success,
                                       destroy_data, list->free_data_func);
    
    // If removing was successful decrease size counter
    if (success) {
        list->size--;
    }
    
    return success;
}

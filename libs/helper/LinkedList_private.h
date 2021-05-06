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


#ifndef LINKEDLIST_PRIV_H
#define LINKEDLIST_PRIV_H


#include "LinkedList.h"

#include <stdbool.h>


static LinkedListNode* LinkedListNode_create_node(void* data);

static void LinkedListNode_destroy_node(LinkedListNode* node, bool destroy_data, void (*free_data_func)(void*));

static LinkedListNode* LinkedListNode_remove(LinkedListNode* list, void* data, bool* success, bool destroy_data, void (*free_data_func)(void*));


#endif

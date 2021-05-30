#include "game_objs.h"
#include <stddef.h>
#include <stdlib.h>

void remove_entity_from_list(entity_list_T *l, entity_list_node_T *entity) {
	/*
		 REFACTOR:
		 I think there is a way to remove one of these temporary
		 nodes; the one I think we can drop is 'next', as next is always
		 accessible via 'curr->next'
	*/
	entity_list_node_T* curr = l->head;
	entity_list_node_T* next = curr->next;
	entity_list_node_T* prev = curr;
	if (l->head == NULL)
		return;
	if (curr == entity) {
		l->head = next;
		free(curr);
	} else {
		do {
			prev = curr;
			curr = next;
			next = next->next;
		} while (curr != entity);
		prev->next = next;
		free(curr);
	}
}

void add_entity_to_list(entity_list_T *l, entity_list_node_T *entity_node) {
	entity_list_node_T* prev_head = l->head;
	l->head = entity_node;
	l->head->next = prev_head;
	return;
}

int entity_list_len(entity_list_T *l) {
	int length = 0;
	entity_list_node_T* curr = l->head;
	while (curr != NULL) {
		length++;
		curr = curr->next;
	}
	return length;
}

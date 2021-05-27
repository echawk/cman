#include "game_objs.h"
#include <stddef.h>
void remove_entity_from_list(entity_list_T *l, entity_list_node_T *entity) {
	entity_list_node_T* curr = l->head;
	entity_list_node_T* next = curr->next;
	entity_list_node_T* prev = curr;
	do {
		prev = curr;
		curr = next;
		next = next->next;
	} while (curr != entity);
	prev->next = curr->next;
	curr = NULL;
	return;
}

void add_entity_to_list(entity_list_T *l, entity_list_node_T *entity_node) {
	entity_list_node_T* current_head = l->head;
	l->head = entity_node;
	l->head->next = current_head;
	return;
}


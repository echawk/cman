#include "game_objs.h"

void remove_entity_from_list(entity_list_T *l, entity_list_node_T *entity) {

	return;
}

void add_entity_to_list(entity_list_T *l, entity_list_node_T *entity_node) {
	entity_list_node_T* current_head = l->head;
	l->head = entity_node;
	l->head->next = current_head;
	return;
}


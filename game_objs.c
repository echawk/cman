#include "game_types.h"

void remove_entity_from_list(entity_list_t *l, entity_t *entity) {

	return;
}

void add_entity_to_list(entity_list_t *l, entity_t *entity) {
	entity_t* current_head = l.head;
	l->head = entity;
	l->head->next = current_head;
	return;
}


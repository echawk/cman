#include "enemy_utils.h"

/*
	 Red is going to move randomly around the map
*/
void update_redenemy_entity(entity_T *entity, int max_y, int max_x) {
	int dx, dy;
	/* generate x */
	if (rand() % 2 == 0) {
		dx = -1;
		if (entity->x + dx < 0) {
			dx = 1;
		}
	} else {
		dx = 1;
		if (entity->x + dx > max_y) {
			dx = 0;
		}
	}

	/* generate y*/
	if (rand() % 2 == 0) {
		dy = -1;
		if (entity->y + dy < 0) {
			dx = 1;
		}
	} else {
		dy = 1;
		if (entity->y + dy > max_y) {
			dx = 0;
		}
	}

	entity->y = entity->y + dy;
	entity->x = entity->x + dx;
}

/*
	Magenta is going to go directly towards the player
*/
void update_magenemy_entity(entity_T *entity, entity_T *player) {
	int dx, dy;
	if (entity->x > player->x) {
		dx = -1;
	} else if (entity->x < player->x) {
		dx = 1;
	} else {
		dx =0;
	}
	if (entity->y > player->y) {
		dy = -1;
	} else if (entity->y < player->y) {
		dy = 1;
	} else {
		dy =0;
	}
	entity->y = entity->y + dy;
	entity->x = entity->x + dx;
}

/*
	 Blue is going to move towards the middle point of the closest powerpill, and the players position
*/
void update_bluenemy_entity(entity_T *entity, entity_T *player, entity_list_T *powerpills, int max_y, int max_x) {




}


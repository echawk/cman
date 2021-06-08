#include "game_utils.h"
#include <stdlib.h>
#include <ncurses.h>

#define NUMPOWERPELLETS 10
#define NUMWALLS 550
/*
	Inputs:
	list - a pointer to a linked list of entities
	player - a pointer to a player entity
	score - a pointer to the score of the game
	ny - the next y value for the player
	nx - the next x value for the player

	This function is responsible for printing out the list of entities, as well as
	performing collision detection. The reason it also does collision detection,
	is because it saves us an iteration through each respective linked list; we
	already have to go through every element once, so we might as well do collision
	detection here as well.

	TODO:
	- add in color support
*/
void print_entity_list(entity_list_T *list, entity_T *player, int *score, int *ny, int *nx) {
	entity_list_node_T *node  = (entity_list_node_T *) malloc(sizeof(entity_list_node_T));
	entity_list_node_T *node2 = (entity_list_node_T *) malloc(sizeof(entity_list_node_T));
	if (list->head == NULL)
		return;
	node = list->head;
	do {
		if (*nx == node->value.x && *ny == node->value.y) {
			/* Handle Collisions based off of entity type */
			switch (node->value.type) {
				case WALL_T:
					*nx = player->x + 0;
					*ny = player->y + 0;
					break;
				case PILL_T:
					*score = *score + 10;
					remove_entity_from_list(list, node);
#ifdef LOG
					fprintf(stderr, "LOG: Score:%d\n", score);
#endif
					break;
				default:
					break;
			}
		}
		mvprintw(node->value.y, node->value.x, node->value.icon);
		node2 = node->next;
		if (node2 != NULL) {
			node  = node2;
		}
	} while (node2 != NULL);
	node  = NULL;
	node2 = NULL;
	/* free(temp);
	   free(temp2);
	*/
}

/*
	Inputs:
	list - a pointer to an empty entity list
	type - the type of entities to create
	icon - the entities icon
	max_y - the max value in the y direction for the game board
	max_x - the max value in the x direction for the game board

	This function takes an empty entity list and fills it up with entities of a specified
	type. It makes a call to set_coordinates to determine the coordinates for each entity
*/
void init_entity_list(entity_list_T *list, int type, char *icon, int max_y, int max_x){
	int i = 0;
	int number_entities = 0;
	switch (type) {
		case PILL_T:
			number_entities = NUMPOWERPELLETS;
			break;
		case WALL_T:
			number_entities = NUMWALLS;
			break;
		default:
			break;
	}
	/* if the list is not empty, return */
	if (list->head != NULL)
		return;
	for (i = 0; i < number_entities; i++) {
		entity_list_node_T* new = (entity_list_node_T*) malloc(sizeof(entity_list_node_T));
		if (new == NULL)
			exit(1);
		new->value.icon = icon;
		new->value.type = type;
		set_coordinates(new, max_y, max_x, list);
		add_entity_to_list(list, new);
		new = NULL;
	}
}

/*
	Inputs:
	new - a pointer to an enitiy list node
	max_y - the max y value
	max_x - the max x value
	list - a pointer to an entity list

	This function is responsible for 'terrain' generation. It takes in an entity and figures
	out where to stick it on the map. For PILL_T entities, it just picks a random coordinate,
	so the list pointer is never accessed. For WALL_T entities, it does some random seeding
	accross the map, but with a higher weight to 'attaching' to the head of the list, wherever
	that may be

	TODO:
	- make the generation more robust for walls; implement a list length function, then pick
	a random inside the list to attach to instead of just the head.
*/
void set_coordinates(entity_list_node_T *new, int max_y, int max_x, entity_list_T *list) {
	new->value.x = (int) rand() % max_x;
	new->value.y = (int) rand() % max_y;
	switch(new->value.type) {
		case PILL_T:
			break;
		case WALL_T:
			/* if it's the first element, just add it */
			if (list->head == NULL)
				break;
			/* 70 % of the time attach to an already existing node */
			if ((int) rand() % 10 <= 7) {
				if ((int) rand() % 2 == 0) {
					new->value.x = list->head->value.x;
					if (list->head->value.y + 1 > max_y) {
						new->value.y = list->head->value.y - 1;
					} else {
						new->value.y = list->head->value.y + 1;
					}
				} else {
					new->value.y = list->head->value.y;
					if (list->head->value.x + 1 > max_x) {
						new->value.x = list->head->value.x - 1;
					} else {
						new->value.x = list->head->value.x + 1;
					}
				}
			}
		default:
			break;
	}
}

/*
	Inputs:
	player - a pointer to an enitiy
	dy - a pointer to store the delta y for the direction
	dx - a pointer to store the delta x for the direction
	direction - a char to determine what direction the player should be heading

	This fuction is responsible for determining both the character sprite for the
	player based off of the direction that is given, but also updates the proper x
	and y deltas for the player to move in the given direction.

	TODO:
*/
void update_player_entity(entity_T *player, short *dy, short *dx, char direction) {
	switch(direction) {
		case 'u':
			player->icon = "v";
			*dx = 0;
			*dy = -1;
			break;
		case 'd':
			player->icon = "^";
			*dx = 0;
			*dy = 1;
			break;
		case 'r':
			player->icon = "<";
			*dx = 1;
			*dy = 0;
			break;
		case 'l':
			player->icon = ">";
			*dx = -1;
			*dy = 0;
			break;
		default:
			player->icon = "o";
			break;
	}
}

#pragma once
#include "game_objs.h"

#define NUMPOWERPELLETS 10
#define NUMWALLS 550

void update_player_entity(entity_T *player, short *dy, short *dx, char direction);
void print_entity_list(entity_list_T *list, entity_T *player, int *score, int *ny, int *nx);
void set_coordinates(entity_list_node_T *new, int max_y, int max_x, entity_list_T *list);
void init_entity_list(entity_list_T *list, int type, char *icon, int max_y, int max_x);
double calc_distance_between_entities(entity_T *en1, entity_T *en2);

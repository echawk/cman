#include "game_objs.h"
#include "game_utils.h"
#include <stdlib.h>

void update_redenemy_entity(entity_T *entity, int max_y, int max_x);
void update_magenemy_entity(entity_T *entity, entity_T *player);
void update_bluenemy_entity(entity_T *entity, entity_T *player, entity_list_T *powerpills, int max_y, int max_x);

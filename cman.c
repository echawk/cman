#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "game_objs.h"

#define DELAY 30000
#define NUMPOWERPELLETS 10
#define NUMWALLS 200

const char EMEMY_CHAR = 'e';
const char WALL_CHAR  = 'w';
const char PILL_CHAR  = 'p';

const char EMEMY_ICON = '@';
const char WALL_ICON  = '#';
const char PILL_ICON  = '*';

/* TODO: move the helper functions to their own header file */

int kbhit(void);
char detdir(char ch);
void update_delts(int *dx, int *dy, char direction);
void update_player_s(char **player_s, char direction);
void print_hwall(int sx, int ex, int y, char wall_c);
void print_vwall(int sy, int ey, int x, char wall_c);
void spawn_power_pellets(int ymax, int xmax);
void init_entity_list_values(entity_list_T *list, char entity_c, int max_y, int max_x);
void init_pill_list(entity_list_T *list, int max_y, int max_x);
void init_wall_list(entity_list_T *list, int max_y, int max_x);

int main(int argc, char *argv[]) {
	int x = 0,
			y = 0;
	int max_y = 0,
			max_x = 0;
	int delta_x = 0,
			delta_y = 0;
	int next_x = 0,
			next_y = 0;
	char direction = 'l'; /*u, d, l, r, n | up down left right none */

	char *player_s = "o";

	time_t t; /* used for srand */

	int score = 0; /* player score */

	entity_list_node_T *temp = (entity_list_node_T *) malloc(sizeof(entity_list_node_T));
	entity_list_node_T *temp2 = (entity_list_node_T *) malloc(sizeof(entity_list_node_T));

	/* Create the lists for the entities */
	entity_list_T *walls = (entity_list_T *) malloc(sizeof(entity_list_T));
	entity_list_T *enemies = (entity_list_T *) malloc(sizeof(entity_list_T));
	entity_list_T *powerpills = (entity_list_T *) malloc(sizeof(entity_list_T));
	walls->head = NULL;
	enemies->head = NULL;
	powerpills->head = NULL;

	srand((unsigned) time(&t));
	initscr();
	cbreak();
	noecho();
	nodelay(stdscr, TRUE);
	curs_set(FALSE);

	getmaxyx(stdscr, max_y, max_x);

	init_entity_list(powerpills, PILL_CHAR, PILL_ICON, max_y, max_x);
	init_entity_list(walls, WALL_CHAR, WALL_ICON, max_y, max_x);

	/*
	Now that we have the max coordinates, we can now fill our
	lists of entities
	*/


	while(1) {
		/* setup stuff */

		clear();
		mvprintw(y, x, player_s);
		/*
		refresh();
		box(stdscr, 0, 0);
		mvprintw(2, 0, "#####");
		mvprintw(10, 0, "#####");
		mvprintw(10, 5, "#####");
		*/

		update_player_s(&player_s, direction);
		update_delts(&delta_x, &delta_y, direction);
		next_x = x + delta_x;
		next_y = y + delta_y;
		if (next_x >= max_x || next_y >= max_y || next_x < 0 || next_y < 0) {
			next_x = x + 0;
			next_y = y + 0;
		}

		/* Print our walls...*/
		/* TODO: refactor; see if these two can be meaningfully merged */
		temp = walls->head;
		do {
			if (next_x == temp->value.x && next_y == temp->value.y) {
				next_x = x + 0;
				next_y = y + 0;
			}
			mvprintw(temp->value.y, temp->value.x, "#");
			temp2 = temp->next;
			temp = temp2;
		} while (temp->next != NULL);

		temp = powerpills->head;
		do {
			if (next_x == temp->value.x && next_y == temp->value.y) {
				score = score + 10;
				fprintf(stderr, "LOG: Score:%d\n", score); /* LOG */
			}
			mvprintw(temp->value.y, temp->value.x, "*");
			temp2 = temp->next;
			temp = temp2;
		} while (temp->next != NULL);
		wrefresh(stdscr);

		x = next_x;
		y = next_y;

		usleep(DELAY);
		if (kbhit()) {
			char ch = getch();
			direction = detdir(ch);
		}
	}

	endwin();
	return 0;
}


void init_entity_list_values(entity_list_T *list, char entity_c, int max_y, int max_x) {
	switch (entity_c) {
		case 'e':
			break;
		case WALL_CHAR:
			init_wall_list(list, max_y, max_x);
			break;
		case PILL_CHAR:
			init_pill_list(list, max_y, max_x);
			break;
		default:
			break;
	}
}


void init_entity_list(entity_list_T *list, char type, char icon, int max_y, int max_x){
	int i = 0;
	int number_entities = 0;
	char desc[20];
	switch (type) {
		case PILL_CHAR:
			number_entities = NUMPOWERPELLETS;
			strcpy(desc, "Power Pill");
			break;
		case WALL_CHAR:
			number_entities = NUMWALLS;
			strcpy(desc, "Wall");
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
		new->value.x = (int) rand() % max_x;
		new->value.y = (int) rand() % max_y;
		new->value.icon = icon;
		new->value.type = type;
		add_entity_to_list(list, new);
		fprintf(stderr, "LOG: Added %s with x:%d and y:%d\n", desc, new->value.x, new->value.y); /* LOG */
		new = NULL;
	}

}

/* TODO: refactor; merge init_pill_list and init_wall list, get rid of init_entity_list_values */
void init_pill_list(entity_list_T *list, int max_y, int max_x) {
	int i = 0;
	/* if the list is not empty, return */
	if (list->head != NULL)
		return;
	for (i = 0; i < NUMPOWERPELLETS; i++) {
		entity_list_node_T* new = (entity_list_node_T*) malloc(sizeof(entity_list_node_T));
		if (new == NULL)
			exit(1);
		new->value.x = (int) rand() % max_x;
		new->value.y = (int) rand() % max_y;
		new->value.icon = PILL_ICON;
		new->value.type = PILL_CHAR;
		add_entity_to_list(list, new);
		fprintf(stderr, "LOG: Added Power Pellet with x:%d and y:%d\n", new->value.x, new->value.y); /* LOG */
		new = NULL;
	}
}

void init_wall_list(entity_list_T *list, int max_y, int max_x) {
	int i = 0;
	if (list->head != NULL)
		return;
	for (i = 0; i < NUMWALLS; i++) {
		entity_list_node_T* new = (entity_list_node_T*) malloc(sizeof(entity_list_node_T));
		if (new == NULL)
			exit(1);
		new->value.x = (int) rand() % max_x;
		new->value.y = (int) rand() % max_y;
		new->value.icon = WALL_ICON;
		new->value.type = WALL_CHAR;
		add_entity_to_list(list, new);
		fprintf(stderr, "LOG: Added Wall with x:%d and y:%d\n", new->value.x, new->value.y); /* LOG */
		new = NULL;
	}

}


/* TODO: merge the update_player_s and update_delts functions */

void update_player_s(char **player_s, char direction) {
	switch(direction) {
		case 'u':
			*player_s = "v";
			break;
		case 'd':
			*player_s = "^";
			break;
		case 'r':
			*player_s = "<";
			break;
		case 'l':
			*player_s = ">";
			break;
		default:
			*player_s = "o";
			break;
	}
}

void update_delts(int *dx, int *dy, char direction) {
	switch(direction) {
		case 'u':
			*dx = 0;
			*dy = -1;
			break;
		case 'd':
			*dx = 0;
			*dy = 1;
			break;
		case 'r':
			*dx = 1;
			*dy = 0;
			break;
		case 'l':
			*dx = -1;
			*dy = 0;
			break;
		default:
			break;
	}
}

/* determines the direction the player should be moving, based on a keypress */
char detdir(char ch){
	switch (ch){
		case 107: /* 'k' - up */
			return 'u';
		case 106: /* 'j' - down */
			return 'd';
		case 108: /* 'l' - right */
			return 'r';
		case 104: /* 'h' - left */
			return 'l';
		default:
			return 'n';
	}
}

/* returns 1 if a key was pressed, 0 otherwise */
int kbhit(){
	int ch = getch();
	if (ch != ERR) {
		ungetch(ch);
		return 1;
	} else {
		return 0;
	}
}

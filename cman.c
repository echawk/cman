#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "game_utils.h"
#include "enemy_utils.h"
#define DELAY 30000

static char *WALL_ICON  = "#";
static char *PILL_ICON  = "*";

/* TODO: move the helper functions to their own header file */

int kbhit(void);
char detdir(char ch);
void update_player_entity(entity_T *player, short *dy, short *dx, char direction);
void print_entity_list(entity_list_T *list, entity_T *player, int *score, int *ny, int *nx);
void set_coordinates(entity_list_node_T *new, int max_y, int max_x, entity_list_T *list);
void init_entity_list(entity_list_T *list, int type, char *icon, int max_y, int max_x);

int main(int argc, char *argv[]) {
	int max_x      = 0, max_y   = 0;
	short delta_x  = 0, delta_y = 0;
	int next_x     = 0, next_y  = 0;
	int score      = 0;
	char direction = 'l'; /*u, d, l, r, n | up down left right none */
	time_t t; /* used for srand */
	unsigned long tick = 0;

	entity_T *redenemy = (entity_T *) malloc(sizeof(entity_T));
	entity_T *magenemy = (entity_T *) malloc(sizeof(entity_T));
	entity_T *bluenemy = (entity_T *) malloc(sizeof(entity_T));

	/* Create the lists for the entities */
	entity_list_T *walls      = (entity_list_T *) malloc(sizeof(entity_list_T));
	/* entity_list_T *enemies    = (entity_list_T *) malloc(sizeof(entity_list_T)); */
	entity_list_T *powerpills = (entity_list_T *) malloc(sizeof(entity_list_T));

	entity_T *player = (entity_T *) malloc(sizeof(entity_T));

	/* All variables are now declared. Now initialize. */

	player->x    = 0;
	player->y    = 0;
	player->icon = "o";

	walls->head = NULL;
	/* enemies->head = NULL; */
	powerpills->head = NULL;

	srand((unsigned) time(&t));
	initscr();
	cbreak();
	noecho();
	nodelay(stdscr, TRUE);
	curs_set(FALSE);

	getmaxyx(stdscr, max_y, max_x);

	/*
	Now that we have the max coordinates, we can now fill our
	lists of entities
	*/
/*
	redenemy->x    = (int) rand() % max_x;
	redenemy->y    = (int) rand() % max_y;
	redenemy->icon = (char) ENEMY_ICON;
*/
	redenemy->x    = 10;
	redenemy->y    = 10;
	redenemy->icon = "@";

	magenemy->x    = 15;
	magenemy->y    = 15;
	magenemy->icon = "@";

	bluenemy->x    = 10;
	bluenemy->y    = 15;
	bluenemy->icon = "@";

	init_entity_list(powerpills, PILL_T, (char*) PILL_ICON, max_y, max_x);
	init_entity_list(walls,      WALL_T, (char*) WALL_ICON, max_y, max_x);

	while(1) {
		/* setup stuff */

		clear();
		start_color();
		init_pair(5, COLOR_YELLOW, COLOR_BLACK);
		attron(COLOR_PAIR(5));
		mvprintw(player->y, player->x, "%s", player->icon);
		attroff(COLOR_PAIR(5));
		update_player_entity(player, &delta_y, &delta_x, direction);
		next_x = player->x + delta_x;
		next_y = player->y + delta_y;
		if (next_x >= max_x || next_y >= max_y || next_x < 0 || next_y < 0) {
			next_x = player->x + 0;
			next_y = player->y + 0;
		}

		/* Print our walls...*/
		/* TODO: refactor; see if these two can be meaningfully merged */

		init_pair(WALL_T, COLOR_BLACK, COLOR_WHITE);
		attron(COLOR_PAIR(WALL_T));
		print_entity_list(walls,      player, &score, &next_y, &next_x);
		attroff(COLOR_PAIR(WALL_T));

		init_pair(PILL_T, COLOR_BLACK, COLOR_GREEN);
		attron(COLOR_PAIR(COLOR_GREEN));
		print_entity_list(powerpills, player, &score, &next_y, &next_x);
		attroff(COLOR_PAIR(COLOR_GREEN));

		init_pair(100, COLOR_BLACK, COLOR_RED);
		attron(COLOR_PAIR(100));
		if (tick % 3 == 0)
			update_redenemy_entity(redenemy, max_y, max_x);
		mvprintw(redenemy->y, redenemy->x, "%s", redenemy->icon);
		attroff(COLOR_PAIR(100));

		init_pair(101, COLOR_BLACK, COLOR_MAGENTA);
		attron(COLOR_PAIR(101));
		if (tick % 5 == 0)
			update_magenemy_entity(magenemy, player);
		mvprintw(magenemy->y, magenemy->x, "%s", magenemy->icon);
		attroff(COLOR_PAIR(101));

		init_pair(102, COLOR_BLACK, COLOR_BLUE);
		attron(COLOR_PAIR(102));
		mvprintw(bluenemy->y, bluenemy->x, "%s", bluenemy->icon);
		attroff(COLOR_PAIR(102));

		wrefresh(stdscr);
		player->x = next_x;
		player->y = next_y;

		usleep(DELAY);
		if (kbhit()) {
			char ch = (char) getch();
			direction = detdir(ch);
		}
		tick++;
	}

	endwin();
	return 0;
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
	char ch = (char) getch();
	if (ch != ERR) {
		ungetch(ch);
		return 1;
	} else {
		return 0;
	}
}

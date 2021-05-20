#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>

#define DELAY 30000
#define NUMPOWERPELLETS 10

/* TODO: move the helper functions to their own header file */

int kbhit(void);
char detdir(char ch);
void update_delts(int *dx, int *dy, char direction);
void update_player_s(char **player_s, char direction);
void print_hwall(int sx, int ex, int y, char wall_c);
void print_vwall(int sy, int ey, int x, char wall_c);
void spawn_power_pellets(int ymax, int xmax);

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

	char *wall_s = "#";
	char wall_c = '#';

	initscr();
	cbreak();
	noecho();
	nodelay(stdscr, TRUE);
	curs_set(FALSE);

	getmaxyx(stdscr, max_y, max_x);

	while(1) {
		/* setup stuff */

		clear();
		//refresh();
		mvprintw(y, x, player_s);
		//box(stdscr, 0, 0);

		mvprintw(2, 0, "#####");
		mvprintw(10, 0, "#####");
		mvprintw(10, 5, "#####");
		wrefresh(stdscr);

		usleep(DELAY);
		if (kbhit()) {
			char ch = getch();
			direction = detdir(ch);
		}
		update_player_s(&player_s, direction);
		update_delts(&delta_x, &delta_y, direction);

		next_x = x + delta_x;
		next_y = y + delta_y;

		if (next_x >= max_x || next_y >= max_y || next_x < 0 || next_y < 0) {
			next_x = x + 0;
			next_y = y + 0;
		}

		x = next_x;
		y = next_y;
	}

	endwin();
	return 0;
}

void gen_power_pellet_coords(int ymax, int xmax) {
	int i = 0;
	int x, y;
	for (i = 0; i < NUMPOWERPELLETS; i++) {
		y = (int) rand() % ymax;
		x = (int) rand() % xmax;
	}
}

void print_hwall(int sx, int ex, int y, char wall_c) {
	int i = 0;
	int diff = ex - sx;
	char *wall_s = (char*) malloc(diff * sizeof(char));
	if (wall_s == NULL)
		exit(1);
	while (i < diff) {
		wall_s[i] = wall_c;
	}
	mvprintw(y, sx, wall_s);
	free(wall_s);
}

void print_vwall(int sy, int ey, int x, char wall_c) {

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

#include <ncurses.h>
#include <unistd.h>

#define DELAY 30000

int kbhit(void);
char detdir(char ch);
void update_delts(int *dx, int *dy, char direction);

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

	initscr();
	cbreak();
	noecho();
	nodelay(stdscr, TRUE);
	curs_set(FALSE);

	getmaxyx(stdscr, max_y, max_x);

	while(1) {
		/* setup stuff */

		clear();
		mvprintw(y, x, "o");
		refresh();

		usleep(DELAY);
		if (kbhit()) {
			char ch = getch();
			direction = detdir(ch);
		}

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

int kbhit(){
	int ch = getch();
	if (ch != ERR) {
		ungetch(ch);
		return 1;
	} else {
		return 0;
	}
}

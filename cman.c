#include <ncurses.h>
#include <unistd.h>

#define DELAY 30000

char kbread();

int main(int argc, char *argv[]) {
	int x = 0, y = 0;
	int max_y = 0, max_x = 0;
	int delta_x = 0, delta_y = 0;
	int next_x = 0, next_y = 0;
	char direction = 'l'; /*u, d, l, r, n | up down left right none */

	initscr();
	cbreak();
	noecho();
	nodelay(stdscr, TRUE);
	curs_set(FALSE);


	while(1) {
		/* setup stuff */
		getmaxyx(stdscr, max_y, max_x);
		clear();

		/*
		printw("c");
		wprintw(stdscr, "x");
		*/
		mvprintw(y, x, "o");
		refresh();
		usleep(DELAY);


		direction = kbread();

		switch(direction) {
			case 'u':
				delta_x = 0;
				delta_y = -1;
			case 'd':
				delta_x = 0;
				delta_y = 1;
			case 'l':
				delta_x = 1;
				delta_y = 0;
			case 'r':
				delta_x = -1;
				delta_y = 0;
			default:
				break;
		}

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

char kbread(){
	int ch = getch();
	if (ch != ERR) {
		ungetch(ch);
		switch (ch){
			case 107: /* 'k' - up */
				return 'u';
			case 106: /* 'j' - down */
				return 'd';
			case 104: /* 'h' - left */
				return 'l';
			case 108: /* 'l' - right */
				return 'r';
		}
	}
	return 'e'; /* 'e' for error */
}

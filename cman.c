#include <ncurses.h>
#include <unistd.h>

#define DELAY 30000

int main(int argc, char *argv[]) {
	int x = 0, y = 0;
	int max_y = 0, max_x = 0;
	int delta_x = 0, delta_y = 0;
	int next_x = 0;
	char direction = 'n'; /*u, d, l, r, n | up down left right none */

	initscr();
	noecho();
	curs_set(FALSE);


	while(1) {
		/* setup stuff */
		getmaxyx(stdscr, max_y, max_x);
		clear();

		printw("c");
		wprintw(stdscr, "x");
		mvprintw(y, x, "o");
		refresh();

		usleep(DELAY);

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
		getch();
		next_x = x + direction;
		if (next_x >= max_x || next_x < 0) {
			direction *= -1;
		} else {
			x+= direction;
		}

	}

	endwin();
	return 0;
}

#include <ncurses.h>
#include <unistd.h>

#define DELAY 30000

int kbhit(void);

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
			printw("Got %c", ch);
			switch (ch){
				case 107: /* 'k' - up */
					direction = 'u';
					break;
				case 106: /* 'j' - down */
					direction = 'd';
					break;
				case 108: /* 'l' - right */
					direction = 'r';
					break;
				case 104: /* 'h' - left */
					direction = 'l';
					break;
				default:
					direction = direction;
					break;
			}
		}

		switch(direction) {
			case 'u':
				delta_x = 0;
				delta_y = -1;
				break;
			case 'd':
				delta_x = 0;
				delta_y = 1;
				break;
			case 'r':
				delta_x = 1;
				delta_y = 0;
				break;
			case 'l':
				delta_x = -1;
				delta_y = 0;
				break;
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

int kbhit(){
	int ch = getch();
	if (ch != ERR) {
		ungetch(ch);
		return 1;
	} else {
		return 0;
	}
}

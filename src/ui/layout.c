#include <ncurses.h>
#include <stdlib.h>

int rows, cols;

void init_layout() {
  getmaxyx(stdscr, rows, cols);
}

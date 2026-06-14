#include <ncurses.h>

void draw_bar(WINDOW *win, int y, int x, int width, int percent) {
  int fill = (percent * width) / 100;

  mvwprintw(win, y, x, "[");
  for (int i = 0; i < width; i++) {
    if (i < fill)
      mvwaddch(win, y, x + 1 + i, '#');
    else
      mvwaddch(win, y, x + 1 + i, '-');
    }
  mvwaddch(win, y, x + width + 1, ']');
}

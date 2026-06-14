#include <ncurses.h>
#include <string.h>

#include "ui/ui_core.h"

void draw_footer() {
  int width = getmaxx(footer_win);
  int footer_width = width / 50;

  const char *nav   = "[Up/Down] Navigate";
  const char *quit  = "[Q] Quit";
  const char *pkill = "[K] Kill by PID";

  mvwhline(footer_win, 0, 1, ACS_HLINE, width - 2);
  mvwhline(footer_win, 2, 1, ACS_HLINE, width - 2);

  mvwprintw(footer_win, 1, footer_width, nav);
  footer_width += strlen(nav) + 2;
  mvwprintw(footer_win, 1, width / 50 + 20, quit);
  footer_width += strlen(quit) + 2;
  mvwprintw(footer_win, 1, width / 50 + 20 + 10, pkill);
  footer_width += strlen(pkill) + 2;

  wrefresh(footer_win);
}

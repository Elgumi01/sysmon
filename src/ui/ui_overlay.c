#include <ncurses.h>

#include "ui/ui_overlay.h"
#include "ui/ui_core.h"

#define DARK_PAIR 1

void toggle_overlay() {
  overlay_win = newwin(LINES, COLS, 0, 0);
  wbkgd(overlay_win, COLOR_PAIR(DARK_PAIR));
  wrefresh(overlay_win);
}

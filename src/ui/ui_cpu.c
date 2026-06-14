#include <ncurses.h>

#include "monitor.h"

#include "ui/ui_core.h"
#include "ui/ui_widgets.h"

void draw_cpu(const Monitor *info) {
  box(cpu_win, 0, 0);

  mvwprintw(cpu_win, 0, 2, "CPU");
  mvwprintw(cpu_win, 2, 25, "%.2f%%", info->cpu_percentage);
  draw_bar(cpu_win, 2, 2, 20, info->cpu_percentage);

  wrefresh(cpu_win);
}

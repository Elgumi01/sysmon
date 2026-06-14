#include <ncurses.h>

#include "monitor.h"

#include "ui/ui_core.h"
#include "ui/ui_widgets.h"

void draw_memory(const Monitor *info) {
  box(mem_win, 0, 0);

  mvwprintw(mem_win, 0, 2, "Memory");
  mvwprintw(mem_win, 2, 25, "%.2f / %.2f Gib", info->used_memory,
                                               info->total_memory);
  draw_bar(mem_win, 2, 2, 20, (info->used_memory * 100) / info->total_memory);

  wrefresh(mem_win);
}

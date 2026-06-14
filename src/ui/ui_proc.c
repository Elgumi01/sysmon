#include <ncurses.h>

#include "monitor.h"

#include "ui/ui_core.h"
#include "ui/input_handler.h"
#include "ui/ui_widgets.h"

void draw_processes(const Monitor *info, int scroll) {
  int rows      = getmaxy(proc_win);
  int cols      = getmaxx(proc_win);
  int start_row = 1;
  int max_lines = rows - 3;

  /* HEADER */
  mvwprintw(proc_win, start_row, 2,  "PID");
  mvwprintw(proc_win, start_row, 10, "COMMAND");
  mvwprintw(proc_win, start_row, cols * 3 / 10 - 1, "MEMB");
  mvwprintw(proc_win, start_row, cols * 1 / 3, "CPU");

  mvwhline(proc_win, start_row + -1, 1, ACS_HLINE, cols - 2);
  mvwhline(proc_win, start_row + 1, 1, ACS_HLINE, cols - 2);

  /* PROCESS LIST */
  for (int i = 0; i < max_lines; i++) {
    int idx = scroll + i;

    if (idx >= info->process_count) break;
    if (i == cursor_pos) wattron(proc_win, A_REVERSE);
    mvwhline(proc_win, start_row + 2 + i, 1, ' ', cols - 2);
    mvwprintw(proc_win, start_row + 2 + i, 2, "%-7d", info->process_list[idx].pid);
    mvwprintw(proc_win, start_row + 2 + i, 10, "%-40s", info->process_list[idx].command);
    mvwprintw(proc_win, start_row + 2 + i, cols * 3 / 10 - 1, "%-6.2f", info->process_list[idx].mem / 1024);
    mvwprintw(proc_win, start_row + 2 + i, cols * 1 / 3, "%-6.2f", info->process_list[idx].cpu);
    if (i == cursor_pos) wattroff(proc_win, A_REVERSE);
  }
  wrefresh(proc_win);
}

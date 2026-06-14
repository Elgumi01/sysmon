#include <ncurses.h>

#include "monitor.h"

#include "ui/ui_core.h"

void draw_system(const Monitor *info) {
  box(sys_win, 0, 0);

  mvwprintw(sys_win, 0, 2, "System");
  mvwprintw(sys_win, 2, 2, "Hostname: %s", info->hostname);
  mvwprintw(sys_win, 3, 2, "User: %s", info->user);
  mvwprintw(sys_win, 4, 2, "Kernel: %s", info->kernel);
  mvwprintw(sys_win, 5, 2, "Shell: %s", info->shell);

  mvwprintw(sys_win, 6, 2, "Uptime: %d h %d m", info->uptime_hours,
                                                info->uptime_minutes);
  mvwprintw(sys_win, 8, 2, "Processes: %d", info->processes);
  mvwprintw(sys_win, 9, 2, "Running: %d", info->running_processes);

  wrefresh(sys_win);
}

#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#include "monitor.h"

#include "ui/ui_core.h"
#include "ui/input_handler.h"
#include "ui/ui_overlay.h"

void draw_proc_kill(pid_t pid, const char *command) {
  toggle_overlay();
  box(proc_kill_win, 0, 0);
  
  mvwprintw(proc_kill_win, 0, 2, "Kill Process");
  mvwprintw(proc_kill_win, 2, 2, "[!] Warning: Kill process?");
  mvwprintw(proc_kill_win, 4, 2, "PID: %d (%s)", pid, command);
  mvwprintw(proc_kill_win, 6, 2, "[ESC] Cancel");
  mvwprintw(proc_kill_win, 7, 2, "[Enter] Confirm");

  wrefresh(proc_kill_win);
}

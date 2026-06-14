#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

#include "monitor.h"

#include "ui/ui_core.h"
#include "ui/ui_proc_kill.h"

#include "config.h"

#define KEY_ESC 27

int scroll_pos    = 0;
int cursor_pos    = 0;
int show_kill_win = 0;

int handle_input(Monitor *info) {
  int key = getch();

  if (key == 'q' || key == 'Q') return 1;

  if (key == 'k' || key == 'K') show_kill_win = !show_kill_win;
  if (key == KEY_ESC) show_kill_win = 0;
  if ((key == '\n' || key == '\r' || key == KEY_ENTER) && show_kill_win == 1) {
    kill(info->process_list[scroll_pos+cursor_pos].pid, SIGKILL);
    show_kill_win = 0;
  }

  if (key == KEY_UP) {
    if (cursor_pos > 0) {
      cursor_pos--;
    } else if (scroll_pos > 0) {
      scroll_pos--;
    }
  }
  if (key == KEY_DOWN) {
    int max_visible = getmaxy(proc_win) - PROC_HEADER_LINES;
    if (cursor_pos < max_visible - 1) {
      cursor_pos++;
    } else if (scroll_pos + max_visible < info->process_count) {
      scroll_pos++;
    }
  }
  if (key == KEY_RESIZE) reinit_windows();

  return 0;
}

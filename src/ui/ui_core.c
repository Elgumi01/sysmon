#include <ncurses.h>
#include <stdlib.h>

#include "ui/layout.h"

#include "config.h"

WINDOW *mem_win;
WINDOW *cpu_win;
WINDOW *sys_win;
WINDOW *proc_win;
WINDOW *footer_win;
WINDOW *proc_kill_win;
WINDOW *overlay_win;

static int proc_y = 0;
static int proc_height = 0;
static int win_height = 0;
static int win_width = 0;

static void update_layout() {
  proc_y      = rows / 4;
  proc_height = rows - proc_y - PROC_HEADER_LINES;
  win_height  = rows / 9;
  win_width   = cols / 4;
}

static void create_windows() {
  mem_win    = newwin(win_height, cols / 4, 1, 1);
  cpu_win    = newwin(win_height, cols / 4, win_height + 2, 1);
  sys_win    = newwin(rows * 10 / 43, cols / 4, 1, cols * 3 / 4);
  proc_win   = newwin(proc_height, cols, proc_y, 0);
  footer_win = newwin(3, cols, rows - 3, 0);

  int proc_kill_win_height = win_height * 2;
  int proc_kill_win_width = cols / 4;
  proc_kill_win = newwin(proc_kill_win_height, proc_kill_win_width, (LINES - proc_kill_win_height) / 2, (COLS - proc_kill_win_width) / 2);
}

void init_ui() {
  initscr();
  noecho();
  curs_set(FALSE);

  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);
  set_escdelay(0);

  init_layout();
  update_layout();

  create_windows();
}

void close_ui() {
  delwin(mem_win);
  delwin(cpu_win);
  delwin(sys_win);
  delwin(proc_win);
  delwin(footer_win);

  endwin();
}

void reinit_windows() {
  init_layout();
  update_layout();

  delwin(mem_win);
  delwin(cpu_win);
  delwin(sys_win);
  delwin(proc_win);
  delwin(footer_win);
  delwin(proc_kill_win);

  clear();
  refresh();

  create_windows();
}

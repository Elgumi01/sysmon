#include <ncurses.h>
#include <signal.h>
#include <stdlib.h>

#include "ui.h"

void init_ui() {
  initscr();
  noecho();
  curs_set(FALSE);

  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);
}

void draw_bar(int y, int x, int width, int percent) {
    int fill = (percent * width) / 100;

    mvprintw(y, x, "[");

    for (int i = 0; i < width; i++) {
        if (i < fill)
            mvaddch(y, x + 1 + i, '#');
        else
            mvaddch(y, x + 1 + i, '-');
    }

    mvaddch(y, x + width + 1, ']');
}

void pid_kill(int rows, int cols) {
  echo();

  WINDOW *win = newwin(3, 40, rows-4, cols-41);
  char input[16];

  box(win, 0, 0);
  mvwprintw(win, 1, 1, "PID: ");
  wrefresh(win);
  wgetstr(win, input);

  pid_t pid = (pid_t)strtol(input, NULL, 10);

  kill(pid, SIGKILL);

  delwin(win);
  noecho();
}

int handle_input(int *scroll_offset, int process_count, int visible_lines, int rows, int cols) {
  int key = getch();
  int max_scroll = process_count - visible_lines;

  if (key == 'q' || key == 'Q') {
    return 1;
  }

  if (key == 'k' || key == 'K') {
    pid_kill(rows, cols);
  }

  if (key == KEY_DOWN) {
    (*scroll_offset)++;
  } 

  if (key == KEY_UP) {
    (*scroll_offset)--;
  }

  if (*scroll_offset < 0) {
    *scroll_offset = 0;
  }

  if (*scroll_offset > max_scroll) {
    *scroll_offset = max_scroll;
  }
  
  return 0;
}

void draw_ui(SystemInfo info, int scroll_offset) {
  int rows, cols;
  getmaxyx(stdscr, rows, cols);
  int process_start_row = 6;

  clear();

  // HEADER

  mvprintw(1,1, "Memory Usage:");
  mvprintw(1,38, "%.2f / %.2f Gib", info.used_memory,
                                   info.total_memory);
  draw_bar(1,15, 20, (info.used_memory * 100) / info.total_memory);
  mvprintw(2,1, "CPU:");
  mvprintw(2,29, "%.2f%%", info.cpu_percentage);
  draw_bar(2,6, 20, info.cpu_percentage);
  mvprintw(3,1, "Uptime: %d h, %d m", info.uptime_hours, info.uptime_minutes);
  
  // PROCESSES

  for (int i = 0; i < (rows - 1) - process_start_row; i++) {

    int process_index = scroll_offset + i;
    
    mvprintw(process_start_row + i, 1, "%-7s %-40s %-8.2f %-6.2f", info.process_list[process_index].pid, 
                                                           info.process_list[process_index].command,
                                                           (info.process_list[process_index].mem) / 1024,
                                                            info.process_list[process_index].cpu);
  }

  // LAYOUT
  mvprintw(5,1, "PID");
  mvprintw(5,9, "COMMAND");
  mvprintw(5,50, "MEMB");
  mvprintw(5,59, "CPU%%");
  mvhline(4, 0, ACS_HLINE, cols);
  box(stdscr, 0, 0);

  refresh();
}

void close_ui() {
  endwin();
}

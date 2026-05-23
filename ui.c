#include <ncurses.h>
#include "ui.h"

void init_ui() {
  initscr();
  noecho();
  curs_set(FALSE);

  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);
}

int handle_input(int *scroll_offset, int process_count, int visible_lines) {
  int key = getch();
  int max_scroll = process_count - visible_lines;

  if (key == 'q' || key == 'Q') {
    return 1;
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
  int process_start_row = 5;

  clear();

  // HEADER

  mvprintw(0,0, "Memory Usage: %.2f / %.2f Gib", info.used_memory, 
                                                 info.total_memory);
  mvprintw(1,0, "CPU: %.2f%%", info.cpu_percentage);
  mvprintw(2,0, "Uptime: %d h, %d m", info.uptime_hours, info.uptime_minutes);

  
  // PROCESSES

  for (int i = 0; i < rows - process_start_row; i++) {

    int process_index = scroll_offset + i;
    
    mvprintw(process_start_row + i, 0, "PID: %s Command: %s", info.process_list[process_index].pid, info.process_list[process_index].command);
  }

  refresh();
}

void close_ui() {
  endwin();
}

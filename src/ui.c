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
  int process_start_row = 6;

  clear();

  // HEADER

  mvprintw(1,1, "Memory Usage: %.2f / %.2f Gib", info.used_memory, 
                                                 info.total_memory);
  mvprintw(2,1, "CPU: %.2f%%", info.cpu_percentage);
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

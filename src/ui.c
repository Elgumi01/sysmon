#include <ncurses.h>
#include <signal.h>
#include <stdlib.h>

#include "ui.h"

// GLOBAL VARIABLES

WINDOW *mem_win;
WINDOW *cpu_win;
/*WINDOW *cpucores_win; */
WINDOW *sys_win;

void init_ui() {
  initscr();
  noecho();
  curs_set(FALSE);

  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);

  mem_win = newwin(5, 42, 1, 1);
  cpu_win = newwin(5, 42, 7, 1);
  sys_win = newwin(12, 40, 1, 130);
  /* cpucores_win = newwin(12, 40, 1, 60); */
}

void draw_bar(WINDOW *win, int y, int x, int width, int percent) {
    int fill = (percent * width) / 100;

    mvwprintw(win, y, x, "[");

    for (int i = 0; i < width; i++) {
        if (i < fill)
            mvwaddch(win, y, x + 1 + i, '#');
        else
            mvwaddch(win, y, x + 1 + i, '-');
    }

    mvwaddch(win, y, x + width + 1, ']');
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
  int process_start_row = 15;

  clear();

  // MEMORY BOX

  werase(mem_win);
  box(mem_win, 0, 0);

  mvwprintw(mem_win, 0, 2, "Memory");
  mvwprintw(mem_win, 2, 25, "%.2f / %.2f Gib", info.used_memory,
                                   info.total_memory);
  draw_bar(mem_win, 2, 2, 20, (info.used_memory * 100) / info.total_memory);

  // CPU
  
  werase(cpu_win);
  box(cpu_win, 0, 0);

  mvwprintw(cpu_win, 0, 2, "CPU");
  mvwprintw(cpu_win, 2, 25, "%.2f%%", info.cpu_percentage);
  draw_bar(cpu_win, 2, 2, 20, info.cpu_percentage);
  
  /* CPU CORES (TODO)
  
  werase(cpucores_win);
  box(cpucores_win, 0, 0);
  
  mvwprintw(cpucores_win, 0, 2, "CORES");
  
  */

  // SYSTEM
  
  werase(sys_win);
  box(sys_win, 0, 0);

  mvwprintw(sys_win, 0, 2, "System");
  mvwprintw(sys_win, 2, 2, "Hostname: %s", info.hostname);
  mvwprintw(sys_win, 3, 2, "User: %s", info.user);
  mvwprintw(sys_win, 4, 2, "Kernel: %s", info.kernel);
  mvwprintw(sys_win, 5, 2, "Shell: %s", info.shell);
  mvwprintw(sys_win, 6, 2, "Uptime: %d h, %d m", info.uptime_hours, info.uptime_minutes);

  mvwprintw(sys_win, 8, 2, "Processes: %d", info.processes);
  mvwprintw(sys_win, 9, 2, "Running: %d", info.running_processes);

  // PROCESSES

  for (int i = 0; i < (rows - 1) - process_start_row; i++) {

    int process_index = scroll_offset + i;
    
    mvprintw(process_start_row + i, 1, "%-7s %-40s %-8.2f %-6.2f", info.process_list[process_index].pid, 
                                                           info.process_list[process_index].command,
                                                           (info.process_list[process_index].mem) / 1024,
                                                            info.process_list[process_index].cpu);
  }

  // LAYOUT
  mvprintw(14,1, "PID");
  mvprintw(14,9, "COMMAND");
  mvprintw(14,50, "MEMB");
  mvprintw(14,59, "CPU%%");
  mvhline(13, 0, ACS_HLINE, cols);
  box(stdscr, 0, 0);
  
  // REFRESH
  
  refresh();
  
  wrefresh(mem_win);
  wrefresh(cpu_win);
  /* wrefresh(cpucores_win); */
  wrefresh(sys_win);
}

void close_ui() {
  delwin(mem_win);
  delwin(cpu_win);
  /* delwin(cpucores_win) */
  delwin(sys_win);
  
  endwin();
}

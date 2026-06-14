#ifndef UI_H
#define UI_H

#include <ncurses.h>

extern WINDOW *mem_win;
extern WINDOW *cpu_win;
extern WINDOW *sys_win;
extern WINDOW *proc_win;
extern WINDOW *footer_win;
extern WINDOW *proc_kill_win;
extern WINDOW *overlay_win;

void init_ui();
void close_ui();
void reinit_windows();

#endif

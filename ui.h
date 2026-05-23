#ifndef UI_H
#define UI_H

#include "system.h"

void init_ui();
int handle_input(int *scroll_offset, int process_count, int visible_lines);
void draw_ui(SystemInfo info, int scroll_offset);
void close_ui();

#endif

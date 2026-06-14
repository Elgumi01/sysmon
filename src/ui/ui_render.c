#include <ncurses.h>

#include "monitor.h"

#include "ui/ui_memory.h"
#include "ui/ui_cpu.h"
#include "ui/ui_system.h"
#include "ui/ui_proc.h"
#include "ui/ui_footer.h"
#include "ui/input_handler.h"
#include "ui/ui_proc_kill.h"

void render_ui(const Monitor *info) {
  box(stdscr, 0, 0);
  wnoutrefresh(stdscr);

  draw_memory(info);
  draw_cpu(info);
  draw_system(info);
  draw_processes(info, scroll_pos);
  draw_footer();

  if (show_kill_win) draw_proc_kill(info->process_list[scroll_pos+cursor_pos].pid,
                                    info->process_list[scroll_pos+cursor_pos].command);

  doupdate();
}

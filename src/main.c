#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>

#include "cpu.h"
#include "memory.h"
#include "uptime.h"
#include "process.h"
#include "ui.h"
#include "system.h"

void update_system_info(SystemInfo *info, CpuStats *old_cpu) {
  // CPU

  CpuStats new_cpu = cpu_usage();
    
  unsigned long long old_idle = old_cpu->idle;
  unsigned long long old_total = cpu_total(*old_cpu);

  unsigned long long new_idle = new_cpu.idle;
  unsigned long long new_total = cpu_total(new_cpu);

  double idle_diff = new_idle - old_idle;
  double total_diff = new_total - old_total;
  
  double cpu_percentage = 100.0 * (1.0 - idle_diff / total_diff); // CPU usage based on idle/total deltas
  
  if (total_diff == 0.0) {
    cpu_percentage = 0.0;
  }

  // MEMORY

  MemoryStatus memory = memory_usage();

  double used_memory = (memory.memtotal - memory.memavailable) / 1024.0 / 1024.0; // Approximate real memory usage in GiB
  double total_memory = memory.memtotal / 1024.0 / 1024.0; // Converts the total memory into GiB

  // UPTIME

  UptimeStats uptime = get_uptime();

  int uptime_hours = uptime.uptime_seconds / 3600;
  int uptime_minutes = ((int)uptime.uptime_seconds % 3600) / 60;
    
  // SYSTEM INFO

  info->cpu_percentage = cpu_percentage;

  info->used_memory = used_memory;
  info->total_memory = total_memory;

  info->uptime_hours = uptime_hours;
  info->uptime_minutes = uptime_minutes;
  
  info->process_count = 0;
  get_processes(info->process_list, &info->process_count, total_diff);
  *old_cpu = new_cpu;
}

int main() {
  init_ui();
  
  int frame_counter = 0;
  int scroll_offset = 0;
  SystemInfo info = {0};
  CpuStats old_cpu = cpu_usage();
  

  while (1) {
    int rows = getmaxy(stdscr);

    int process_start_row = 6;

    int visible_lines = rows - process_start_row;

    if (handle_input(&scroll_offset, info.process_count, visible_lines)) {
      break;
    }

    if (frame_counter % 60 == 0) {
      update_system_info(&info,
                         &old_cpu);
    }

    draw_ui(info, scroll_offset);

    usleep(16000);

    frame_counter++;
  }

  close_ui();

  return 0;
}

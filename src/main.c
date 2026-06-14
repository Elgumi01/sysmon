#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>

#include "cpu.h"
#include "memory.h"
#include "system_info.h"
#include "process.h"
#include "monitor.h"
#include "limits.h"

#include "ui/ui_core.h"
#include "ui/ui_render.h"
#include "ui/input_handler.h"
#include "ui/layout.h"

void update_system_info(Monitor *info, CpuStats *old_cpu) {
  /* CPU */

  CpuStats new_cpu = cpu_usage();

  unsigned long long old_idle  = old_cpu->idle;
  unsigned long long old_total = cpu_total(*old_cpu);

  unsigned long long new_idle  = new_cpu.idle;
  unsigned long long new_total = cpu_total(new_cpu);

  double idle_diff = new_idle - old_idle;
  double total_diff = new_total - old_total;

  double cpu_percentage = 100.0 * (1.0 - idle_diff / total_diff); // CPU usage based on idle/total deltas

  if (total_diff == 0.0) {
    cpu_percentage = 0.0;
  }

  /* MEMORY */

  MemoryStatus memory = memory_usage();

  double used_memory  = (memory.memtotal - memory.memavailable) / 1024.0 / 1024.0; // Approximate real memory usage in GiB
  double total_memory = memory.memtotal / 1024.0 / 1024.0; // Converts the total memory into GiB

  /* SYSTEM INFO */

  SystemInfo systeminfo = get_system_info();

  int uptime_hours = systeminfo.uptime_seconds / 3600;
  int uptime_minutes = ((int)systeminfo.uptime_seconds % 3600) / 60;

  char hostname[HOSTNAME_BUFFER] = {0};
  snprintf(hostname, sizeof(hostname), systeminfo.hostname);

  char user[USER_BUFFER] = {0};
  snprintf(user, sizeof(user), systeminfo.user);
 
  char kernel[KERNEL_BUFFER] = {0};
  snprintf(kernel, sizeof(kernel), systeminfo.kernel);

  char shell[SHELL_BUFFER] = {0};
  snprintf(shell, sizeof(shell), systeminfo.shell);

  int processes = systeminfo.processes;
  int running_processes = systeminfo.running_processes;

  /* PASSING SYSTEM TO UI */

  info->cpu_percentage = cpu_percentage;

  info->used_memory = used_memory;
  info->total_memory = total_memory;

  info->uptime_hours = uptime_hours;
  info->uptime_minutes = uptime_minutes;
  info->processes = processes;
  info->running_processes = running_processes;

  info->process_count = 0;
  get_processes(info->process_list, &info->process_count, total_diff);
  *old_cpu = new_cpu;

  snprintf(info->hostname, sizeof(info->hostname), hostname);
  snprintf(info->user, sizeof(info->user), user);
  snprintf(info->kernel, sizeof(info->kernel), kernel);
  snprintf(info->shell, sizeof(info->shell), shell);
}

int main() {
  init_ui();

  int frame_counter = 0;
  Monitor info = {0};
  CpuStats old_cpu = cpu_usage();

  while (1) {
    if (handle_input(&info)) {
      break;
    }
    if (frame_counter % 60 == 0) {
    update_system_info(&info,
                         &old_cpu);
    }
    render_ui(&info);

    usleep(16000);
    frame_counter++;
  }

  close_ui();

  return 0;
}

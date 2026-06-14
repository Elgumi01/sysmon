#ifndef SYSTEM_H
#define SYSTEM_H

#include "config.h"

#include "process.h"

typedef struct {
  double cpu_percentage;

  double used_memory, total_memory;

  int uptime_hours, uptime_minutes;

  Process process_list[PROCESS_LIST_BUFFER];
  int process_count, processes, running_processes;

  char hostname[HOSTNAME_BUFFER], user[USER_BUFFER], kernel[KERNEL_BUFFER], shell[SHELL_BUFFER];

} Monitor;

#endif

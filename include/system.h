#ifndef SYSTEM_H
#define SYSTEM_H

#include "process.h"

typedef struct {
  double cpu_percentage;

  double used_memory, total_memory;

  int uptime_hours, uptime_minutes;

  Process process_list[4096];
  int process_count, processes, running_processes;

  char hostname[256], user[256], kernel[256], shell[256];

} SystemInfo;

#endif

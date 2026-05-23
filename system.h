#ifndef SYSTEM_H
#define SYSTEM_H

#include "process.h"

typedef struct {
  double cpu_percentage;

  double used_memory;
  double total_memory;

  int uptime_hours;
  int uptime_minutes;

  Process process_list[3000];
  int process_count;

} SystemInfo;

#endif

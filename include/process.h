#ifndef PROCESS_H
#define PROCESS_H

#include <sys/types.h>

#include "config.h"

typedef struct {
  pid_t pid;
  char command[PROCESS_COMMAND_BUFFER];

  double mem, cpu;

  unsigned long long old_proc_cpu;
} Process;

void get_processes(Process process_list[], int *process_count, double delta_total_cpu);

#endif

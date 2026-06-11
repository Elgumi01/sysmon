#ifndef PROCESS_H
#define PROCESS_H

typedef struct {
  char pid[256], command[256];

  double mem, cpu;
  
  int processes, running_processes;

  unsigned long long old_proc_cpu;
} Process;

void get_processes(Process process_list[], int *process_count, double delta_total_cpu);

#endif

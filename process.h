#ifndef PROCESS_H
#define PROCESS_H

typedef struct {
  char pid[256];
  char command[256];
} Process;

void get_processes(Process process_list[], int *process_count);

#endif

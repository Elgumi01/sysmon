#include <stdio.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "config.h"

#include "process.h"

#define PATH_BUFFER 512
#define LINE_BUFFER 256

void get_processes(Process process_list[], int *process_count, double delta_total_cpu) {
  DIR *proc = opendir("/proc");

  if (proc == NULL) {
    printf("Error opening /proc\n");
    return;
  }

  struct dirent *entry;

  int process_index = 0;

  while ((entry = readdir(proc)) != NULL) {

    if (process_index >= PROCESS_LIST_BUFFER) break;
    if (!isdigit(entry->d_name[0])) continue;

    char comm_path[PATH_BUFFER] = {0};
    char mem_path[PATH_BUFFER] = {0};
    char cpu_path[PATH_BUFFER] = {0};

    snprintf(comm_path, sizeof(comm_path), "/proc/%s/comm", entry->d_name);
    snprintf(mem_path, sizeof(mem_path), "/proc/%s/status", entry->d_name);
    snprintf(cpu_path, sizeof(cpu_path), "/proc/%s/stat", entry->d_name);

    FILE *f_command = fopen(comm_path, "r");
    FILE *f_memory = fopen(mem_path, "r");
    FILE *f_cpu = fopen(cpu_path, "r");

    if (f_command == NULL || f_memory == NULL || f_cpu == NULL) {
      if (f_command) fclose(f_command);
      if (f_memory) fclose(f_memory);
      if (f_cpu) fclose(f_cpu);

      continue;
    }

    /* COMMAND */

    char command_line[LINE_BUFFER] = {0};
    char process_command[LINE_BUFFER] = {0};
    if (fgets(command_line, sizeof(command_line), f_command) == NULL) {
      fprintf(stderr, "Error opening /proc/PID/comm\n");
      fclose(f_command);
      return;
    }
    sscanf(command_line, "%255s", process_command);

    /* MEMORY */

    char memory_line[LINE_BUFFER] = {0};
    unsigned long long process_memory = 0;

    while (fgets(memory_line, sizeof(memory_line), f_memory)) {
      if (strncmp(memory_line, "VmRSS:", 6) == 0) {
        sscanf(memory_line, "VmRSS: %llu kB", &process_memory);
        break;
      }
    }

    /* CPU */

    char cpu_line[LINE_BUFFER] = {0};
    unsigned long long process_utime = 0;
    unsigned long long process_stime = 0;


    if (fgets(cpu_line, sizeof(cpu_line), f_cpu) == NULL) {
      fprintf(stderr, "Error opening /proc/PID/stat\n");
      fclose(f_cpu);
      return;
    }
    sscanf(cpu_line,
        "%*d (%*255[^)]) %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %llu %llu",
        &process_utime,
        &process_stime
    );

    unsigned long new_proc_time = process_utime + process_stime;
    unsigned long old_proc_time = process_list[process_index].old_proc_cpu;

    int pid = (int)strtol(entry->d_name, NULL, 10);

    if (old_proc_time == 0) process_list[process_index].cpu = 0;

    unsigned long delta_proc = new_proc_time - old_proc_time;

    process_list[process_index].old_proc_cpu = new_proc_time;

    if (delta_total_cpu > 0) {
        double cpu_percent = 100.0 * delta_proc / delta_total_cpu;
        process_list[process_index].cpu = cpu_percent;
    }

    /* SAVING */

    process_list[process_index].pid = pid;

    snprintf(process_list[process_index].command,
             sizeof(process_list[process_index].command),
             "%s", process_command);

    process_list[process_index].mem = process_memory;

    process_index++;

    fclose(f_command);
    fclose(f_memory);
    fclose(f_cpu);
  }

  closedir(proc);

  *process_count = process_index;
}

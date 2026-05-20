#include <stdio.h>
#include <dirent.h>
#include <ctype.h>

#include "process.h"

void get_processes(Process process_list[], int *process_count) {
  DIR *proc = opendir("/proc");

  if (proc == NULL) {
    printf("Error opening /proc\n");
    return;
  }

  struct dirent *entry;

  int process_index = 0;

  while ((entry = readdir(proc)) != NULL) { // Reads /proc files(PIDs + system dirs)

    if (process_index >= 3000) break; // Limit number of process collected
    if (!isdigit(entry->d_name[0])) continue; // Only keep numeric dirs(PIDs)

    char pid_path[512] = {0};
    char line[512] = {0};
    char process_command[256] = {0};

    snprintf(pid_path, sizeof(pid_path), "/proc/%s/comm", entry->d_name); // Build PID path to get the command name

    FILE *f = fopen(pid_path, "r");

    if (f == NULL) {
      continue;
    }

    fgets(line, sizeof(line), f);
    sscanf(line, "%255s", process_command);

    snprintf(process_list[process_index].pid,
             sizeof(process_list[process_index].pid),
             "%s", entry->d_name); // Save PID value to process_list struct

    snprintf(process_list[process_index].command,
             sizeof(process_list[process_index].command),
             "%s", process_command); // Save command name to process_list struct

    process_index++;

    fclose(f);
  }
  
  closedir(proc);

  *process_count = process_index; // Give process_count the process_index value to limit the process showing
}

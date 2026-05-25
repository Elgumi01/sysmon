#include <stdio.h>
#include <string.h>

#include "memory.h"

MemoryStatus memory_usage() {
  FILE *f_memory = fopen("/proc/meminfo", "r");

  MemoryStatus memory = {0};

  if (f_memory == NULL) {
    fprintf(stderr, "Error opening /proc/meminfo\n");
    fclose(f_memory);
    return memory;
  }
  
  char line[256] = {0};
  
  while (fgets(line, sizeof(line), f_memory)) {
    if (strncmp(line, "MemTotal:", 9) == 0) {
      sscanf(line, "MemTotal: %llu kB", &memory.memtotal);
    } else if (strncmp(line, "MemAvailable:", 9) == 0) {
      sscanf(line, "MemAvailable: %llu kB", &memory.memavailable);
      break;
    }
  }

  fclose(f_memory);

  return memory;
}

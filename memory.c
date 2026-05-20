#include <stdio.h>

#include "memory.h"

MemoryStatus memory_usage() {
  FILE *f = fopen("/proc/meminfo", "r");

  MemoryStatus memory = {0};

  if (f == NULL) {
    printf("Error opening /proc/meminfo\n");
    return memory;
  }
  
  char line1[256];
  char line2[256];
  char line3[256];
  line1[0] = 0;
  line2[0] = 0;
  line3[0] = 0;

  fgets(line1, sizeof(line1), f);
  sscanf(line1, "MemTotal: %llu kB", &memory.memtotal);
  fgets(line2, sizeof(line2), f);
  fgets(line3, sizeof(line3), f);
  sscanf(line3, "MemAvailable: %llu kB", &memory.memavailable);

  fclose(f);

  return memory;
}

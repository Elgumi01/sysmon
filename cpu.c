#include <stdio.h>

#include "cpu.h"

CpuStats cpu_usage() {
  FILE *f_cpu = fopen("/proc/stat", "r");
  CpuStats cpu = {0};
  
  if (f_cpu == NULL) {
    fprintf(stderr, "Error opening /proc/stat\n");
    fclose(f_cpu);
    return cpu;
  }

  char line_cpu[256];
  fgets(line_cpu, sizeof(line_cpu), f_cpu);
  int fields = sscanf(line_cpu, "cpu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
         &cpu.user, &cpu.nice, &cpu.system, &cpu.idle, 
         &cpu.iowait, &cpu.irq, &cpu.softirq, &cpu.steal, &cpu.guest);
  
  if (fields != 9) {
    fprintf(stderr, "Error parsing /proc/stat (got %d fields)\n", fields);
    fclose(f_cpu);
    return cpu;
  }

  fclose(f_cpu);
  return cpu;
}

unsigned long long cpu_total(CpuStats cpu) {
  return cpu.user +
         cpu.nice +
         cpu.system +
         cpu.idle +
         cpu.iowait +
         cpu.irq +
         cpu.softirq +
         cpu.steal +
         cpu.guest;
}

#include <stdio.h>

#include "cpu.h"

CpuStats cpu_usage() {
  FILE *fp = fopen("/proc/stat", "r");
  CpuStats cpu = {0};
  
  if (fp == NULL) {
    printf("Error opening /proc/stat\n");
    return cpu;
  }

  char line_cpu[256];
  fgets(line_cpu, sizeof(line_cpu), fp);
  sscanf(line_cpu, "cpu %llu %llu %llu %llu %llu %llu %llu",
         &cpu.user, &cpu.nice, &cpu.system, &cpu.idle, 
         &cpu.iowait, &cpu.irq, &cpu.softirq);

  fclose(fp);
  return cpu;
}

unsigned long long cpu_total(CpuStats cpu) {
  return cpu.user +
         cpu.nice +
         cpu.system +
         cpu.idle +
         cpu.iowait +
         cpu.irq +
         cpu.softirq;
}

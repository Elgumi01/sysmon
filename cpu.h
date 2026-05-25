#ifndef CPU_H
#define CPU_H

typedef struct {
  unsigned long long user, nice, system, idle, iowait, irq, softirq, steal, guest;
} CpuStats;

CpuStats cpu_usage();
unsigned long long cpu_total(CpuStats cpu);

#endif

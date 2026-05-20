#include <stdio.h>
#include <unistd.h>

#include "cpu.h"
#include "memory.h"

int main() {
  while (1) {
    // CPU
    
    CpuStats old_cpu = cpu_usage();

    sleep(1);

    CpuStats new_cpu = cpu_usage();
    
    unsigned long long old_idle = old_cpu.idle;
    unsigned long long old_total = cpu_total(old_cpu);

    unsigned long long new_idle = new_cpu.idle;
    unsigned long long new_total = cpu_total(new_cpu);

    double idle_diff = new_idle - old_idle;
    double total_diff = new_total - old_total;

    double cpu_percentage = 100.0 * (1.0 - idle_diff / total_diff);
    
    // MEMORY

    MemoryStatus memory = memory_usage();
    
    double used_memory = (memory.memtotal - memory.memavailable) / 1024.0 / 1024.0;
    double memory_total = memory.memtotal / 1024.0 / 1024.0;
    
    printf("\033[H\033[J");

    printf("CPU Usage: %.2f%%\n", cpu_percentage);
    printf("Memory Usage: %.2f GiB / %.2f GiB\n", used_memory, memory_total);
  }
}

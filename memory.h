#ifndef MEMORY_H
#define MEMORY_H

typedef struct {
  unsigned long long memtotal, memavailable;
} MemoryStatus;

MemoryStatus memory_usage();

#endif 



#ifndef SYSTEMINFO_H
#define SYSTEMINFO_H

typedef struct {
  double uptime_seconds, idle_seconds;
  char hostname[256], user[256], kernel[256], shell[256];
  int processes, running_processes;

} SystemStats;

SystemStats get_system_info();

#endif

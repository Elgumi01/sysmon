#ifndef SYSTEMINFO_H
#define SYSTEMINFO_H

#include "config.h"

typedef struct {
  double uptime_seconds, idle_seconds;
  char hostname[HOSTNAME_BUFFER], user[USER_BUFFER], kernel[KERNEL_BUFFER], shell[SHELL_BUFFER];
  int processes, running_processes;

} SystemInfo;

SystemInfo get_system_info();

#endif

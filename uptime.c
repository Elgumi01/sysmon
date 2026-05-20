#include <stdio.h>
#include "uptime.h"

UptimeStats get_uptime() {
  FILE *f = fopen("/proc/uptime", "r");

  UptimeStats stats = {0};

  if (f == NULL) {
    printf("Error opening /proc/uptime");
    return stats;
  }

  char line[256];

  fgets(line, sizeof(line), f);

  sscanf(line, "%lf %lf", &stats.uptime_seconds, &stats.idle_seconds);

  fclose(f);

  return stats;
}

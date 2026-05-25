#include <stdio.h>
#include "uptime.h"

UptimeStats get_uptime() {
  FILE *f_uptime = fopen("/proc/uptime", "r");

  UptimeStats stats = {0};

  if (f_uptime == NULL) {
    fprintf(stderr, "Error opening /proc/uptime\n");
    fclose(f_uptime);
    return stats;
  }

  char line[256];

  fgets(line, sizeof(line), f_uptime);

  if(sscanf(line, "%lf %lf", &stats.uptime_seconds, &stats.idle_seconds) != 2) {
    fprintf(stderr, "Error parsing /proc/uptime\n");
    fclose(f_uptime);
    return stats;
  } 

  fclose(f_uptime);

  return stats;
}

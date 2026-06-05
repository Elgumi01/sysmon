#ifndef UPTIME_H
#define UPTIME_H

typedef struct {
  double uptime_seconds, idle_seconds;
} UptimeStats;

UptimeStats get_uptime();

#endif

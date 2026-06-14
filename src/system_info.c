#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <string.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <dirent.h>
#include <ctype.h>

#include "system_info.h"

#define CHAR_BUFFER 256

SystemInfo get_system_info() {
  FILE *f_uptime = fopen("/proc/uptime", "r");
  FILE *f_stat = fopen("/proc/stat", "r");
  DIR *d_proc = opendir("/proc");

  SystemInfo stats = {0};

  if (f_uptime == NULL) {
    fprintf(stderr, "Error opening /proc/uptime\n");
    return stats;
  }
  
  if (f_stat == NULL) {
    fprintf(stderr, "Error opening /proc/stat\n");
    return stats;
  }

  if (d_proc == NULL) {
    fprintf(stderr, "Error opening /proc\n");
    return stats;
  }

  char uptime_line[CHAR_BUFFER] = {0};

  if (fgets(uptime_line, sizeof(uptime_line), f_uptime) == NULL) {
    fprintf(stderr, "Error reading /proc/uptime\n");
    fclose(f_uptime);
    return stats;
  }

  /* HOSTNAME */

  gethostname(stats.hostname, sizeof(stats.hostname));

  /* USER */

  struct passwd *pw = getpwuid(getuid());

  snprintf(stats.user, sizeof(stats.user), pw->pw_name);

  /* KERNEL */

  struct utsname uts;

  if (uname(&uts) == -1) {
    fprintf(stderr, "Error getting kernel info via uname\n");
    return stats;
  }

  snprintf(stats.kernel, sizeof(stats.kernel), uts.release);


  /* SHELL */

  char *shell = getenv("SHELL");

  char *name = strrchr(shell, '/');

  if (name) {
    name++;
  }

  snprintf(stats.shell, sizeof(stats.shell), name);

  /* PROCESSES */

  struct dirent *entry;
  int processes = 0;

  while ((entry = readdir(d_proc)) != NULL) {
    if (isdigit(entry->d_name[0])) {
      processes++;
    }
  }

  stats.processes = processes;

  /* PROCESSES RUNNING */

  int running_processes;
  char running_line[CHAR_BUFFER] = {0};

  while (fgets(running_line, sizeof(running_line), f_stat)) {
    if (sscanf(running_line, "procs_running %d", &running_processes) == 1) {
      break;
    }
  }

  stats.running_processes = running_processes;

  /* UPTIME */

  if(sscanf(uptime_line, "%lf %lf", &stats.uptime_seconds, &stats.idle_seconds) != 2) {
    fprintf(stderr, "Error parsing /proc/uptime\n");
    fclose(f_uptime);
    return stats;
  }

  fclose(f_uptime);
  fclose(f_stat);
  closedir(d_proc);

  return stats;
}

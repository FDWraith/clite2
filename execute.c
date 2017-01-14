#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <errno.h>
#include <sys/stat.h>

void exec_dot(char * cmd) {
  if (strcmp(cmd, ".quit") == 0)
    printf("true");  // quit won't work without this print statement for some reason...
    exit(0);
}

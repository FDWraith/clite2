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

#include "process.h"
#include "execute.h"

int main(int argc, char *argv[]) {

  // checks for file parameter
  if (argc != 2) {
    printf("clite2 requires exactly one file parameter\n");
    exit(0);
  }
  
  int fd = open(argv[1], O_CREAT | O_RDWR);
  // prints command prompt
  printf("> ");
  
  // 
  char s[1000];
  while(fgets(s, sizeof(s), stdin)) {
    /*printf("%s", s);
    if (strcmp ((char *) s[0], '.') == 0)*/
    exec_dot(s);
    printf("> ");
  }
  
  return 0;
}

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
#include "utils.h"

// mode variables
int echo;

void updateModes(int modes[]) {
  echo = modes[0];
}

int main(int argc, char *argv[]) {

  // checks for file parameter
  if (argc != 2) {
    printf("clite2 requires exactly one file parameter\n");
    exit(0);
  }
  
  // creates new database file
  //int fd = open(argv[1], O_CREAT | O_RDWR);
  createFileIfNotExists(argv[1]);
  
  
  // prints command prompt
  printf("Welcome to clite2!\nEnter \".help\" for instructions.\nEnd all SQL commands with a \";\".\n");
  printf("> ");
  
  // processing inputted commands
  char s[1000];  // storing command in string s
  while(fgets(s, sizeof(s), stdin)) {
    // gets rid of trailing newline
    s[strlen(s)-1] = 0;
    
    // handles modes
    /*if (strstr(s, ".") && (strstr(s, "ON") || strstr(s, "OFF"))) {
      int update[];
      memcpy(exec_mode(s), update, 12);
      updateModes(update);
    }*/
      
    // handles . commands
    if (strstr(s, ".")) {
      exec_dot(s);
    }else{
      exec_shell_cmd(s, argv[1]);
    }
    if (echo) printf("%s\n", s);
    // prints command prompt after commands are inputted
    printf("> ");
  }
  
  return 0;
}

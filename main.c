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
#include <sys/wait.h>

#include "process.h"
#include "execute.h"
#include "utils.h"

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
    
    // handles . commands
    if (strstr(s, ".") && (strstr(s, "tables")) == NULL) {
      if( strstr(s, ".exit" ) != NULL || strstr(s, ".quit") != NULL ){
        exit(0);
      }else{
        int status;
        int f = fork();
        if( f == 0 ){
          exec_dot(s);
          exit(0);
        }else{
          wait(&status);
          printf("> ");
        }
      }
    }
    else{
      int status;
      int f = fork();
      if (f == 0){
        exec_shell_cmd(s, argv[1]);
        exit(0);
      }else{
        wait(&status);
        printf("> ");
      }
    }
  }
  
  return 0;
}

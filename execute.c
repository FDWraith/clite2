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
#define STND_SIZE 1000;

char * stripWhiteSpace(char ** originalString);

void exec_dot(char * cmd) {
  if (strcmp(cmd, ".quit") == 0) {
    //printf("true");  // quit won't work without this print statement for some reason...
    exit(0);
  }
}


void exec_shell_cmd( char * cmd ) {
  if( strstr(cmd, "CREATE TABLE") != NULL ){
    struct data_table * table = (struct data_table *)malloc( sizeof( struct data_table ) );
    cmd = strstr(cmd, "CREATE TABLE") + strlen("CREATE TABLE");
    while( *cmd == ' ' ){
      cmd++;
    }
    char tablename[256];
    int counter = 0;
    while( *cmd != ' '){
      tablename[counter] = *cmd;
      cmd++;
      counter++;
    }
    tablename[counter] = 0;
    (*table).TABLENAME = tablename;
    counter = 0;
    char * valueString = findStringPair( &cmd, "(", ")");
    while( valueString ){
      char * tempString = strsep(&valueString, ",");
      if( strchr( tempString, ' ') == NULL){
        printf("CLite Error: Missing Type\n");
        exit(0);
      }else{
        (*table).HEADERS[counter]  = strsep(&tempString, " ");
        while( *tempString == ' ' ){
          tempString++;
        }
        char type[256];
        int i = 0;
        while( *tempString != ' '){
          type[i] = *tempString;
          tempString++;
          i++;
        }
        if( strcmp( type, "TEXT" )) {
          (*table).TYPES[counter] = type;
        }else if( strcmp( type, "INTEGER" ) ){
          (*table).TYPES[counter] = type;
        }else{
          printf("CLite Error: Type Not Found\n");
          exit(0);
        }
        counter++;
      }
    }
  }
}


char * stripWhiteSpace( char ** originalString ){
  while( (**originalString) == ' ' ){
    (*originalString)++;
  }
  while( (*originalString)[strlen(*originalString)-1] == ' '){
    (*originalString)[strlen(*originalString)-1] = 0;
  }
  return (*originalString);
}

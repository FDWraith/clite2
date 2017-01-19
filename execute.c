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

#include "utils.h"

void exec_dot(char * cmd) {
  
  char * help = "this is the help menu";
  
  // quit and exit
  if (strcmp(cmd, ".quit") == 0 || strcmp(cmd, ".exit") == 0) {
    exit(0);
  }
  
  // help
  else if (strcmp(cmd, ".help") == 0) {
    printf("%s\n", help);
  }
  
  // print
  else if (strstr(cmd, ".print")) {
    if (strstr(cmd, "\"")) {
      stripWhiteSpace(&cmd);
      printf("did it work? %s\n", cmd);
    }
    strsep(&cmd, " ");
    printf("%s\n", cmd);
  }
  
  // echo
  else if (strstr(cmd, ".echo")) {
    strsep(&cmd, " ");
    if (strcmp(cmd, "on") == 0)
      return 1;
    else if (strcmp(cmd, "off") == 0)
      return 0;
  }
  
  // invalid command
  else {
    printf("invalid command\n");
  }
}

int exec_mode(char * cmd) {
  
}

void exec_shell_cmd( char * cmd, char * filename ) {
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
    stripWhiteSpace(&valueString);
    while( valueString ){
      char * tempString = strsep(&valueString, ",");
      stripWhiteSpace(&tempString);
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
    struct database * db = readDatabase(filename);
    //NUM_OF_TABLES is not working, so I have to compute length manually
    counter = 0;
    while( (*db).TABLENAMES[counter] != NULL ){
      counter++;
    }
    (*db).TABLENAMES[counter] = tablename;
    (*db).DATATABLES[counter] = *table;
    (*db).NUM_OF_TABLES++;
    writeDatabase( *db, filename );
  }else if( strstr(cmd, "SELECT") != NULL){
    char * desirables = findStringPair( &cmd, "SELECT", "FROM" );
    stripWhiteSpace( &desirables );
    if( strcmp(desirables, "*")){
      //Print whole table? As soon as I find out which table it is.
      stripWhiteSpace( &cmd );
      struct database * db = readDatabase(filename);
      char ** listOfTables = (char **)malloc(sizeof(char *) * 20 + STND_SIZE );
      int counter = 0;
      while( cmd && strcmp( cmd, "" ) != 0 ) {
        char * tempString = strsep(&cmd, ",");
        stripWhiteSpace(&tempString);
        listOfTables[counter] = tempString;
        counter++;
      }
      int numOfTables = counter;
      printDatabase( db, listOfTables, numOfTables );
    }else{
      //specific params that we are looking for T_T
      stripWhiteSpace(&cmd);
      struct database * db = readDatabase(filename);
      char ** listOfTables = (char **)malloc(sizeof(char *) * 20 + STND_SIZE );
      int counter = 0;
      while( cmd && strcmp( cmd, "" ) != 0 ) {
        char * tempString = strsep(&cmd, ",");
        stripWhiteSpace(&tempString);
        listOfTables[counter] = tempString;
        counter++;
      }
      int numOfTables = counter;
      struct data_table * tableList = (struct data_table *)malloc( sizeof(struct data_table) * 50 );
      for(counter = 0; counter < numOfTables; counter++ ){
        int j = 0;
        while( (*db).TABLENAMES[j] ){
          if( strcmp( (*db).TABLENAMES[j] , listOfTables[counter] ) == 0 ){
            tableList[counter] = (*db).DATATABLES[j];
            break;
          }else{
            j++;
          }
        }
      }

      char ** sumHeaders;
      
      while( desirables ) {
        
      }
    }
  }else if( strstr(cmd, "DELETE" ) != NULL ){
    
  }
}


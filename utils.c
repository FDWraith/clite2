#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <errno.h>

#include "process.h"

char * stripWhiteSpace( char ** originalString );
char * findStringPair( char ** originalString, char * firstDenom, char * secondDenom);
void lock();
void unlock();
void printDatabase( struct database * db, char ** listOfTableNames, int numOfTables );
void printTable( struct data_table * tb);

char * stripWhiteSpace( char ** originalString ){
  char whiteSpace[10] = { ' ', '\n', ';'};
  int i = 0;
  for(;i<3;i++){
    while( (**originalString) == whiteSpace[i] ){
      (*originalString)++;
    }
    while( (*originalString)[strlen(*originalString)-1] == whiteSpace[i]){
      (*originalString)[strlen(*originalString)-1] = 0;
    }
  }
  return (*originalString);
}

char * removeChar (char ** originalString, char c) {
  char newStr[strlen(*originalString)-1];
  int i = 0;
  int n = 0;
  for(; i < strlen(*originalString) - 1; i++) {
    if ((*originalString)[i] != c) {
      newStr[n] = (*originalString)[i];
      n++;
    }
  }
  newStr[n] = 0;
  strcpy(*originalString, newStr);
  return (*originalString);
}

void lock(){
  int semid;
  int key = ftok("makefile", 55);
  struct sembuf sb;
  semid = semget( key, 1, 0 );
  sb.sem_op = -1;
  sb.sem_num = 1;
  sb.sem_flg = SEM_UNDO;
  semop(semid, &sb, 1);
}

void unlock(){
  int semid;
  int key = ftok("makefile", 55);
  struct sembuf sb;
  semid = semget( key, 1, 0 );
  sb.sem_num = 1;
  sb.sem_flg = SEM_UNDO;
  sb.sem_op = 1;
  semop(semid, &sb, 1);
}

char * findStringPair(char ** originalString, char * firstDenom, char * secondDenom ){
  //printf("OriginalString:[%s]\n", *originalString);
  // printf("First Denom:[%s]\n", firstDenom);
  char * result = strstr(*originalString, firstDenom)+strlen(firstDenom);
  char * temp = strstr(*originalString, secondDenom)+strlen(secondDenom);
  *strstr(result, secondDenom) = 0;
  *originalString = temp;
  return result;  
}

void printDatabase( struct database * db, char ** listOfTableNames, int numOfTables ){
  int counter = 0;
  int i = 0;
  for(; counter < numOfTables; counter++){
    while( (*db).TABLENAMES[i] != NULL && strcmp(listOfTableNames[counter], (*db).TABLENAMES[i] ) != 0 ){
      i++;
    }
    if( (*db).TABLENAMES[i] == NULL ){
      printf("Table %s Not Found\n", listOfTableNames[counter] );
          exit(0);
    }else if( strcmp(listOfTableNames[counter], (*db).TABLENAMES[i] ) == 0){
      //print out table.
      printTable( &((*db).DATATABLES[i]) );
    }else{
      printf("Table %s Does Not Match\n", listOfTableNames[counter] );
    }
  }
}

void printTable( struct data_table * tb){
  /*____________________________*
   *                            *
   * TINA, PLEASE FILL THIS OUT *
   *____________________________*
   *                            *
  */  
}

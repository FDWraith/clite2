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

char * stripWhiteSpace( char ** originalString );
char * findStringPair( char ** originalString, char * firstDenom, char * secondDenom);
void lock();
void unlock();

char * stripWhiteSpace( char ** originalString ){
  while( (**originalString) == ' ' ){
    (*originalString)++;
  }
  while( (*originalString)[strlen(*originalString)-1] == ' '){
    (*originalString)[strlen(*originalString)-1] = 0;
  }
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

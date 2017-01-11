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

#define STND_SIZE 1000

struct data_entry {
  char * TYPE;
  int INT_VAL;
  char * TEXT_VAL;
};

struct data_table {
  char * TABLENAME;
  char * * HEADERS;
  char * * TYPES;
  struct data_entry * * VALUES;
};

int openFileAttempt(char * filename);
struct data_table * readTables(int fd);
void lock();
void unlock();
char * findStringPair( char ** originalString, char * firstDenom, char * secondDenom);
struct data_table turnStringToTable(char ** fullString, char * tablename);

void lock(){
  int semid;
  int key = ftok("makefile", 55);
  struct sembuf sb;
  semid = semget( key, 1, 0 );
  sb.sem_op = -1;
  sb.sem_num = 1;
  sb.sem_flg = SEM_UNDO;
  semop(semid, &sb, 1);
  printf("inside semaphore\n");
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
  printf("Exited semaphore\n");
}

int openFileAttempt( char * filename ){
  //int err;
  int * fd = (int *)(malloc( sizeof(int) ));
  lock();
  umask(0);
  printf("attempting file open\n");
  *fd = open(filename, O_CREAT | O_RDWR);
  printf("file opened\n");
  unlock();
  return *fd;
}

struct data_table * readTables( int fd ){
  struct stat buf;
  struct data_table * tables;
  int err;
  lock();
  err = fstat( fd, &buf);
  if( err < 0 ){
    printf("Something went wrong with fstat:%s\n", strerror(errno));
    return 0;
  }
  char * fullString;
  err = read(fd, &fullString, buf.st_size );//read size of file into fullString
  if( err < 0 ){
    printf("Something went wrong with read:%s\n", strerror(errno));
    return 0;
  }  
  char * dbInfo = findStringPair(&fullString, "<DATABASE_INFO>", "<DATABASE_INFO_END>");
  char * tableList = strsep(&dbInfo, "!");
  int numOfTables = atoi( strsep(&dbInfo, "!") );
  tables = calloc(numOfTables, sizeof(struct data_table) );
  int counter = 0;
  while( tableList ){
    char * currentTable = strsep( &tableList, "|");
    struct data_table * singleTable = (struct data_table *)(malloc( sizeof( struct data_table *) ));
    if( strcmp(currentTable, "") != 0 ){
      *singleTable = turnStringToTable( &fullString, currentTable );
      tables[counter] = *singleTable;
      counter++;
    }
  }
  unlock();
  return tables;
}

struct data_table turnStringToTable( char ** fullString, char * tablename ){ // Assumes all the proper parts in string

  struct data_table * table = (struct data_table *)malloc( sizeof( struct data_table ));
  (*table).TABLENAME =  tablename;
  
  char denom1[STND_SIZE];
  char denom2[STND_SIZE];
  sprintf(denom1, "<TABLE:%s>", tablename);
  sprintf(denom2, "<TABLE_END:%s>", tablename);
  
  char * tableString = findStringPair(fullString, denom1, denom2);
  
  *denom1 = 0;
  *denom2 = 0;
  sprintf(denom1, "<TABLE_INFO:%s>", tablename);
  sprintf(denom2, "<TABLE_INFO_END:%s>", tablename);

  char * tbInfo = findStringPair(&tableString, denom1, denom2);
  
  char * * tableTypes = (char * *)(malloc( sizeof(char *) * STND_SIZE));
  int counter = 0;
  while( tbInfo ){
    char * singleType = strsep(&tbInfo, "|");
    if( strcmp(singleType, "") != 0 ){
      tableTypes[counter] = singleType;
    }
    counter++;
  }
  (*table).TYPES = tableTypes;
  
  *denom1 = 0;
  *denom2 = 0;
  sprintf(denom1, "<TABLE_HEADERS:%s>", tablename);
  sprintf(denom2, "<TABLE_HEADERS_END:%s>", tablename);
  
  char * tbHeaders = findStringPair(&tableString, denom1, denom2);
  
  char * * tableHeaders = (char * *)(malloc( sizeof(char *) * STND_SIZE));
  counter = 0;
  while( tbHeaders ){
    char * singleHeader = strsep(&tbHeaders, "|");
    if( strcmp(singleHeader, "") != 0 ){
      tableHeaders[counter] = singleHeader;
    }
    counter++;
  }
  (*table).HEADERS = tableHeaders;
  
  *denom1 = 0;
  *denom2 = 0;
  sprintf(denom1, "<TABLE_DATA:%s>", tablename);
  sprintf(denom2, "<TABLE_DATA_END:%s>", tablename);
  
  struct data_entry * * tableValues = (struct data_entry * *)malloc(sizeof(struct data_entry *) * STND_SIZE);
  
  int c = 0;
  while( tableString != NULL && strcmp(tableString, "") != 0){
    char * dataRow = findStringPair(&tableString, denom1, denom2);
    if( strcmp(dataRow, "") != 0 ){
      struct data_entry * valueRow = (struct data_entry *)malloc(sizeof(struct data_entry) * STND_SIZE);
      counter = 0;
      while( dataRow ){
        char * dtValue = strsep(&dataRow, "|");
        if( strcmp(dtValue, "") != 0 ){
          struct data_entry * dataEntry = (struct data_entry *)malloc(sizeof(struct data_entry *));
          (*dataEntry).TYPE = *((*table).TYPES+counter);
          if( strcmp((*dataEntry).TYPE, "TEXT") == 0 ){
            (*dataEntry).TEXT_VAL = dtValue;
          }else if( strcmp((*dataEntry).TYPE, "INTEGER") == 0 ){
            (*dataEntry).INT_VAL = atoi(dtValue);
          }else{
            printf("TYPE NOT SUPPORTED, SOMETHING WENT WRONG\n");
            break;
          }
          valueRow[counter] = *dataEntry;
          counter++;
        }
      }
      tableValues[c] = valueRow;
      c++;
    }    
  }
  
  return *table;
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


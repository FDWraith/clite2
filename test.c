#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "process.h"

int main(){
  printf("Beginning Test for Reading From File\n");

  printf("=========================\n");
  
  printf("Testing findStringPair\n\n");
  
  char * fullString = (char *)malloc(1000);
  strcpy(fullString, "<TABLE:TABLENAME>Blah Blah<TABLE_END:TABLENAME>SomethingElse");
  char denom1[256];
  strcpy(denom1, "<TABLE:TABLENAME>");
  char denom2[256];
  strcpy(denom2, "<TABLE_END:TABLENAME>");
  printf("\tFullString:[%s]\n", fullString);
  printf("\tdenom1:[%s]\n", denom1);
  printf("\tdenom2:[%s]\n", denom2);
  printf("\nfindStringPair output:[%s]\n", findStringPair(&fullString, denom1, denom2));
  printf("Result of FullString after OP:[%s]\n", fullString);

  printf("\nTesting Complete, Moving on\n");

  printf("=========================\n");

  printf("Testing turnStringToTable\n\n");

  strcpy(fullString, "<TABLE:TABLENAME><TABLE_INFO:TABLENAME>TEXT|TEXT|TEXT<TABLE_INFO_END:TABLENAME><TABLE_HEADERS:TABLENAME>LOL|Whut|RANDOM<TABLE_HEADERS_END:TABLENAME><TABLE_DATA:TABLENAME>rip|second|tres<TABLE_DATA_END:TABLENAME><TABLE_END:TABLENAME>");
  char tablename[256];
  strcpy(tablename, "TABLENAME");
  printf("\tFullString:[%s]\n", fullString);
  printf("\tTableName:[%s]\n", tablename);

  struct data_table table = turnStringToTable( &fullString, tablename);
  
  printf("Resulting Table information:\n");
  printf("\tTableName:[%s]\n", table.TABLENAME);
  printf("\tFirst Table Header:[%s]\n", table.HEADERS[0]);
  printf("\tFirst Table Type:[%s]\n", table.TYPES[0]);
  struct data_entry * firstRow = table.VALUES[0];
  struct data_entry firstEntry = firstRow[0];
  struct data_entry secondEntry = firstRow[1];
  printf("\tFirst Table Entry:[%s]\n", firstEntry.TEXT_VAL);
  printf("\tSecond Table Header:[%s]\n", table.HEADERS[1]);
  printf("\tSecond Table Type:[%s]\n", table.TYPES[1]);
  printf("\tSecond Table Entry:[%s]\n", secondEntry.TEXT_VAL);
  printf("Result of FullString after OP:[%s]\n", fullString);

  printf("\nTesting Complete, Moving on\n");

  printf("=========================\n");

  printf("Testing openFileAttempt and readDatabase\n\n");

  int fd = openFileAttempt("testfile");
  printf("File Opened:[%d]\n", fd);

  struct database db = *readDatabase(fd);
  int numOfTables = (db).NUM_OF_TABLES;
  printf("numOfTables[%d]\n", numOfTables);
  int i = 0;
  for(;i<numOfTables;i++){
    struct data_table table = (db).DATATABLES[i];
    printf("Table information for Table %d\n",i);
    printf("\tTableName:[%s]\n", table.TABLENAME);
    printf("\tFirst Table Header:[%s]\n", table.HEADERS[0]);
    printf("\tFirst Table Type:[%s]\n", table.TYPES[0]);
    struct data_entry * firstRow = table.VALUES[0];
    struct data_entry firstEntry = firstRow[0];
    struct data_entry secondEntry = firstRow[1];
    printf("\tFirst Table Entry:[%s]\n", firstEntry.TEXT_VAL);
    printf("\tSecond Table Header:[%s]\n", table.HEADERS[1]);
    printf("\tSecond Table Type:[%s]\n", table.TYPES[1]);
    printf("\tSecond Table Entry:[%s]\n", secondEntry.TEXT_VAL);
  }
  

  
  //int fd = openFileAttempt("testfile");
  
}

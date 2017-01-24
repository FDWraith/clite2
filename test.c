#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "process.h"
#include "execute.h"
#include "utils.h"

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

  struct database * db = readDatabase("testfile");
  int * numOfTables = (*db).NUM_OF_TABLES;
  printf("numOfTables[%d]\n", *numOfTables);
  int i = 0;
  for(;i<*numOfTables;i++){
    struct data_table table = (*db).DATATABLES[i];
    if( table.TABLENAME != NULL ){
      printf("Table information for Table %d\n",i);
      printf("\tTableName:[%s]\n", table.TABLENAME);
      printf("\tFirst Table Header:[%s]\n", table.HEADERS[0]);
      printf("\tFirst Table Type:[%s]\n", table.TYPES[0]);
      struct data_entry * firstRow = table.VALUES[0];
      if( firstRow != 0 ){
        struct data_entry firstEntry = firstRow[0];
        struct data_entry secondEntry = firstRow[1];
        printf("\tFirst Table Entry:[%s]\n", firstEntry.TEXT_VAL);
        printf("\tSecond Table Header:[%s]\n", table.HEADERS[1]);
        printf("\tSecond Table Type:[%s]\n", table.TYPES[1]);
        printf("\tSecond Table Entry:[%s]\n", secondEntry.TEXT_VAL);
      }
    }else{
      break;
    }
  }

  printf("\nTesting Complete, Moving on\n");

  printf("=========================\n");

  printf("Testing turnTableToString\n\n");

  char * result = turnTableToString(table);

  printf("FullString: [%s]\n", result);

  printf("\nTesting Complete, Moving on\n");

  printf("========================\n");

  printf("Testing writeDatabase\n\n");

  writeDatabase( *db, "secondfile");

  printf("\nTesting Complete, Moving on\n");

  printf("========================\n");

  printf("Testing stripWhiteSpace\n\n");

  char * string = (char *)malloc(sizeof(char) * 256);
  strcpy(string, "   Three White to left, Two White to Right  ");
  printf("String Before: [%s]\n", string);
  char * returnV = stripWhiteSpace(&string);
  printf("String Now: [%s]\n", string);

  printf("\nTesting Complete, Moving on\n");

  printf("========================\n");

  printf("Testing Table Creation\n\n");
  
  strcpy(string, "CREATE TABLE blank ( riperino TEXT )");
  printf("testString:[%s]\n", string);
  exec_shell_cmd( string, "secondfile");
  strcpy(string, "CREATE TABLE wow ( rip TEXT, id INTEGER )");
  printf("secondTestString:[%s]\n", string);
  exec_shell_cmd(string, "secondfile");

  printf("\nTesting Complete, Moving on\n");

  printf("========================\n");

  printf("Testing Table Selection\n\n");

  strcpy(string, "SELECT * FROM soemthing" );
  printf("testString:[%s]\n", string);
  exec_shell_cmd( string, "secondfile");
  strcpy(string, "SELECT H1 FROM soemthing" );
  printf("testString:[%s]\n", string);
  exec_shell_cmd(string, "secondfile");
  strcpy(string, "SELECT H1, H2 FROM soemthing");
  printf("testString:[%s]\n", string);
  exec_shell_cmd(string, "secondfile");
  strcpy(string, "SELECT H1, LOL FROM soemthing, TABLENAME");
  printf("testString:[%s]\n", string);
  exec_shell_cmd(string, "secondfile");
  return 0;
}

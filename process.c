#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "utils.h"

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

struct database{
  char * * TABLENAMES;
  int * NUM_OF_TABLES;
  struct data_table * DATATABLES;
};

struct database * readDatabase(char * filename);
struct data_table turnStringToTable(char ** fullString, char * tablename);
char * turnTableToString( struct data_table table );
void writeDatabase( struct database, char * filename );
void createFileIfNotExists( char * filename );

void createFileIfNotExists( char * filename ){
  lock();
  umask(0000);
  int fd = open( filename, O_CREAT | O_EXCL | O_WRONLY, 0644 );
  if (fd > 0 ) {
    char string[STND_SIZE] = "<DATABASE_INFO>!0<DATABASE_INFO_END>";
    write(fd, string, strlen(string));
    close(fd);
  }
  unlock();
}

struct database * readDatabase( char * filename ){
  lock();
  umask(0);
  int fd = open(filename, O_CREAT | O_RDWR );
  struct stat buf;
  struct data_table * tables;
  char ** tablenames = (char **)malloc(sizeof(char *) * STND_SIZE);
  struct database * db = (struct database *)calloc(1, sizeof (struct database) + STND_SIZE);
  int err;
  err = fstat( fd, &buf);
  if( err < 0 ){
    printf("Something went wrong with fstat:%s\n", strerror(errno));
  }
  char * fullString = (char *)malloc( buf.st_size + STND_SIZE);
  err = read(fd, fullString, buf.st_size );//read size of file into fullString
  if( err < 0 ){
    printf("Something went wrong with read:%s\n", strerror(errno));
  }
  char * dbInfo = findStringPair(&fullString, "<DATABASE_INFO>", "<DATABASE_INFO_END>");
  char * tableList = strsep(&dbInfo, "!");
  printf("dbInfo:[%s]\n", dbInfo );
  int * pt = (int *)malloc(sizeof(int));
  *pt = atoi(dbInfo);
  (*db).NUM_OF_TABLES = pt;
  //printf("NUM_OF_TABLES:[%d]\n", (*db).NUM_OF_TABLES);
  tables = calloc(*(*db).NUM_OF_TABLES, sizeof(struct data_table) );
  int counter = 0;
  while( tableList ){
    char * currentTable = strsep( &tableList, "|");
    struct data_table * singleTable = (struct data_table *)(malloc( sizeof( struct data_table *) ));
    if( strcmp(currentTable, "") != 0 ){
      tablenames[counter] = currentTable;
      *singleTable = turnStringToTable( &fullString, currentTable );
      tables[counter] = *singleTable;
      counter++;
    }
  }
  (*db).DATATABLES = tables;
  (*db).TABLENAMES = tablenames;
  close(fd);
  unlock();
  return db;
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
  
  struct data_entry * * tableValues = (struct data_entry * *)calloc(1,sizeof(struct data_entry *) * STND_SIZE);
  
  int c = 0;
  while( tableString != NULL && strcmp(tableString, "") != 0 && strstr(tableString, "<TABLE_DATA") != NULL){
    char * dataRow = findStringPair(&tableString, denom1, denom2);
    if( strcmp(dataRow, "") != 0 ){
      struct data_entry * valueRow = (struct data_entry *)malloc(sizeof(struct data_entry) * STND_SIZE);
      counter = 0;
      while( dataRow ){
        char * dtValue = strsep(&dataRow, "|");
        if( strcmp(dtValue, "") != 0 ){
          struct data_entry * dataEntry = (struct data_entry *)malloc(sizeof(struct data_entry) * 256);
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

  (*table).VALUES = tableValues;
  
  return *table;
}

char * turnTableToString( struct data_table table ){
  char * string = (char *)malloc( sizeof(table) + STND_SIZE);
  char * tablename = table.TABLENAME;
  char ** headers = table.HEADERS;
  char ** types = table.TYPES;
  struct data_entry ** values = table.VALUES;
  int length = 0;
  length += sprintf(string+length, "<TABLE:%s>", tablename);

  int counter = 0;
  length += sprintf(string+length, "<TABLE_INFO:%s>", tablename);
  while( types[counter] ){
    if( types[counter + 1]){
      length += sprintf(string+length, "%s|", types[counter]);
    }else{
      length += sprintf(string+length, "%s", types[counter]);
    }
    counter++;
  }
  length += sprintf(string+length, "<TABLE_INFO_END:%s>", tablename);
  
  counter = 0;
  length += sprintf(string+length, "<TABLE_HEADERS:%s>", tablename);
  while( headers[counter] ){
    if( headers[counter + 1]){
      length += sprintf(string+length, "%s|", headers[counter]);
    }else{
      length += sprintf(string+length, "%s", headers[counter]);
    }
    counter++;
  }
  length += sprintf(string+length, "<TABLE_HEADERS_END:%s>", tablename);

  counter = 0;
  while( values[counter] ){
    length += sprintf(string + length, "<TABLE_DATA:%s>", tablename);
    struct data_entry * valueRow = values[counter];
    int i = 0;
    while( &valueRow[i] != NULL  && valueRow[i].TYPE ){
      if( valueRow[i+1].TYPE ){
        struct data_entry current = valueRow[i];
        if( strcmp(current.TYPE, "TEXT") == 0){
          length += sprintf(string+length, "%s|", current.TEXT_VAL);
        }else if( strcmp( current.TYPE, "INTEGER") == 0){
          length += sprintf(string+length, "%d|", current.INT_VAL);
        }else{
          printf("Someone done goofed. Type not fouund!\n");
        }
      }else{
        struct data_entry current = valueRow[i];
        if( strcmp(current.TYPE, "TEXT") == 0){
          length += sprintf(string+length, "%s", current.TEXT_VAL);
        }else if( strcmp( current.TYPE, "INTEGER") == 0 ){
          length += sprintf(string+length, "%d", current.INT_VAL);
        }else{
          printf("Someone done goofed. Type not fouund!\n");
        }
      }
      i++;
    }
    length += sprintf(string+length, "<TABLE_DATA_END:%s>", tablename);
    counter++;
  }

  length += sprintf(string+length, "<TABLE_END:%s>", tablename);
  return string;
}

void writeDatabase( struct database db, char * filename ){
  lock();
  umask(0);
  int fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY , 0644);
  int length = 0;
  char * string = (char *)malloc( sizeof( db ) + STND_SIZE );
  length += sprintf(string, "<DATABASE_INFO>");
  int counter = 0;
  while( db.TABLENAMES[counter] ){
    if ( db.TABLENAMES[counter + 1] ) {
      length += sprintf(string+length, "%s|", db.TABLENAMES[counter]);
    }else{
      length += sprintf(string+length, "%s!", db.TABLENAMES[counter]);
    }
    counter++;
  }
  length += sprintf(string+length, "%d<DATABASE_INFO_END>", counter );
  counter = 0;
  while( db.TABLENAMES[counter] ){
    length += sprintf(string+length, "%s", turnTableToString(db.DATATABLES[counter]) );
    counter++;
  }

  printf("Final String:[%s]\n", string);

  write( fd, string, strlen(string));
  unlock();
}

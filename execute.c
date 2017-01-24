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
#include "process.h"

#define STND_SIZE 1000

int exec_dot(char * cmd) {
  
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
      removeChar(&cmd, '\"');
    }
    strsep(&cmd, " ");
    printf("%s\n", cmd);
  }
  
  // invalid command
  else {
    printf("invalid command\n");
  }
  
  return 0;
}

void exec_shell_cmd( char * cmd, char * filename ) {
  stripWhiteSpace(&cmd);
  if( strstr(cmd, "CREATE TABLE") != NULL ){
    struct data_table * table = (struct data_table *)calloc(1, sizeof( struct data_table ) );
    cmd = strstr(cmd, "CREATE TABLE") + strlen("CREATE TABLE");
    stripWhiteSpace(&cmd);
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
    char ** headers = (char ** )malloc( sizeof(char *) * STND_SIZE);
    char ** types = (char **)malloc( sizeof( char *) * STND_SIZE);
    while( valueString ){
      char * tempString = strsep(&valueString, ",");
      stripWhiteSpace(&tempString);
      if( strchr( tempString, ' ') == NULL){
        printf("CLite Error: Missing Type\n");
        exit(0);
      }else{
        headers[counter] = strsep(&tempString, " ");
        //printf("what's here? [%s]\n", (*table).HEADERS );
          //(*table).HEADERS[counter] = strsep(&tempString, " ");// Error here. 
        stripWhiteSpace(&tempString);
        char * type = (char *)malloc(sizeof(char) * 256);
        strcpy( type, tempString);
        //printf("type:[%s]\n", type);
        if( strcmp( type, "TEXT" ) == 0) {
          types[counter] = type;
        }else if( strcmp( type, "INTEGER" ) == 0){
          types[counter] = type;
        }else{
          printf("CLite Error: Type Not Found\n");
          exit(0);
        }
        //printf("types[counter] < [%s]\n", types[counter]);
        counter++;
      }
    }
    (*table).HEADERS = headers;
    (*table).TYPES = types;
    (*table).VALUES = (struct data_entry **)calloc(STND_SIZE ,sizeof(data_entry) * STND_SIZE );
    struct database * db = readDatabase(filename);
    lock();
    //NUM_OF_TABLES is not working, so I have to compute length manually
    counter = 0;
    while( (*db).TABLENAMES[counter] != NULL ){
      counter++;
    }
    (*db).TABLENAMES[counter] = tablename;
    (*db).DATATABLES[counter] = *table;
    (*db).NUM_OF_TABLES++;
    unlock();
    writeDatabase( *db, filename );
  }else if( strstr(cmd, "SELECT") != NULL){
    char * desirables = findStringPair( &cmd, "SELECT", "FROM" );
    stripWhiteSpace( &desirables );
    if( strcmp(desirables, "*") == 0){
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
      char ** sumHeaders = (char **)malloc(sizeof(char *) * STND_SIZE);
      int i = 0;
      counter = 0;
      //printf("numOfTables[%d]\n", numOfTables);
      for(;i<numOfTables;i++){
        int j = 0;
        while( tableList[i].HEADERS[j] ){
          //printf("tableList[i].TABLENAME: %s\n", tableList[i].TABLENAME);
          char * string = (char *)malloc(sizeof(char) * STND_SIZE );
          sprintf(string, "%s.%s", tableList[i].TABLENAME, tableList[i].HEADERS[j]);
          sumHeaders[counter] = string;
          counter++;
          j++;
        }
      }

      int lengthOfSum = counter;
      counter = 0;
      char ** desirableHeaders = (char **)malloc(sizeof(char *) * STND_SIZE);
      while( desirables ) {
        char * desirable = strsep(&desirables, ",");
        stripWhiteSpace(&desirable);
        int checker = 0;
        for( i=0; i<lengthOfSum; i++ ){
          //printf("checker: [%s]\n", sumHeaders[i]);
          if( strstr(sumHeaders[i], desirable) != NULL){
            if( checker == 0 ){
              char * desirableHeader = (char *)malloc(sizeof(char) * STND_SIZE);
              strcpy(desirableHeader, sumHeaders[i]);
              desirableHeaders[counter] = desirableHeader;              
              counter++;
              checker = 1;
            }else{
              printf("Please specify the table to which %s belongs\n", desirable);
              exit(0);
            }
          }
        }
        if( checker == 0 ){
          printf("Header %s cannot be found\n", desirable);
          exit(0);
        }
      }
      
      int lengthOfDesirables = counter;
      struct data_table * newTables = (struct data_table *)malloc( sizeof(struct data_table) * 20);
      for( counter = 0; counter < numOfTables; counter++){
        struct data_table * newTable = (struct data_table *)malloc( sizeof(struct data_table) );
        char * * newHeaders = (char * *)malloc( sizeof( char * ) * STND_SIZE);
        char * * newTypes = (char * *)malloc( sizeof(char *) * STND_SIZE);
        struct data_entry * * newValues = (struct data_entry * * )malloc( sizeof( struct data_entry) * STND_SIZE * STND_SIZE);
        int j = 0;
        for( i=0; i<lengthOfDesirables; i++ ){
          char * desirable = (char *)malloc(sizeof(char) * STND_SIZE);
          strcpy(desirable, desirableHeaders[i]);
          //printf("desirableHeader [%s]\n", desirable);
          char * tblname = strsep(&desirable,".");
          if( strcmp( tblname, tableList[counter].TABLENAME ) == 0 ){
            (*newTable).TABLENAME = tblname;
            //printf("desirable [%s]\n", desirable);
            newHeaders[j] = desirable;//Desirable is currently the rest of the header :D
            int k = 0;
            while( tableList[counter].HEADERS[k] ){
              if( strcmp( tableList[counter].HEADERS[k], desirable ) == 0 ){
                newTypes[j] = tableList[counter].TYPES[k];
                int l = 0;
                while( tableList[counter].VALUES != NULL && tableList[counter].VALUES[l] != NULL){
                  if( newValues[l] == NULL ){
                    struct data_entry * valueRow = (struct data_entry *)malloc(sizeof(struct data_entry) * STND_SIZE);
                    newValues[l] = valueRow;
                  }
                  //printf("l:[%d], j:[%d], k:[%d]\n", l, j, k);
                  //printf("checking if VALUES[l] exists: [%p]\n", &tableList[counter].VALUES[l][k] ); 
                  newValues[l][j] = tableList[counter].VALUES[l][k];
                  l++;
                }
                break;
              }else{
                k++;
              }
            }
            j++;
          }
        }
        (*newTable).HEADERS = newHeaders;
        (*newTable).TYPES = newTypes;
        (*newTable).VALUES = newValues;
        newTables[counter] = *newTable;
      }
     
      struct database * newDB = (struct database *)malloc( sizeof(struct database) );
      (*newDB).TABLENAMES = listOfTables;
      (*newDB).DATATABLES = newTables;
      (*newDB).NUM_OF_TABLES = &numOfTables;
      printDatabase( newDB, listOfTables, numOfTables );//Finally complete
    }
  }else if( strstr(cmd, "DELETE" ) != NULL ){
    cmd = strstr(cmd, "DELETE") + strlen("DELETE");
    stripWhiteSpace(&cmd);
    char ** listOfTables = (char **)malloc(sizeof(char *) * 20 + STND_SIZE );
    int counter = 0;
    while( cmd && strcmp( cmd, "" ) != 0 ) {
        char * tempString = strsep(&cmd, ",");
        //printf("tempString:[%s]\n", tempString);
        stripWhiteSpace(&tempString);
        listOfTables[counter] = tempString;
        counter++;
    }
    
    int numOfTables = counter;
    
    struct database * db = readDatabase(filename);    
    struct data_table * tables = (*db).DATATABLES;
    struct data_table * newTables = (struct data_table *)malloc( sizeof(struct data_table) * 20);  //updated tables
    char * * newTableNames = (char * *)malloc( sizeof(char *) * STND_SIZE);
    
    int i = 0;
    for(; i< numOfTables; i++ ){
      char * removeTable = listOfTables[i];
      
      // populating new, updated list newTables
      counter = 0;
      int j = 0;
      while( (*db).TABLENAMES[counter] ){
        struct data_table * newTable = (struct data_table *)malloc( sizeof(struct data_table) );
        if (strcmp( removeTable, (*db).TABLENAMES[counter] ) != 0) {
          *newTable = tables[counter];
          newTables[j] = *newTable;
          newTableNames[j] = (*db).TABLENAMES[counter];
          j++;
        }
        counter++;
      }
      
    }    
    // updating database struct
    (*db).DATATABLES = newTables;
    (*db).TABLENAMES = newTableNames;
    writeDatabase(*db, filename);
  }
  
  else if ( strstr(cmd, "INSERT") != NULL ) {
    struct database * db = readDatabase(filename);
    strsep(&cmd, " ");  // get rid of "INSERT"
    //printf("cmd: %s\n", cmd);
    strsep(&cmd, " ");  // get rid of "INTO"
    //printf("cmd: %s\n", cmd);
    char * tableName = strsep(&cmd, " ");  // remove table name and store it
    strsep(&cmd, " ");  // get rid of "VALUES"
    //printf("cmd: %s\n", cmd);
    cmd = findStringPair(&cmd, "(", ")"); // gets rid of parentheses
    //printf("strip cmd: %s\n", cmd);
    
    // putting values obtained from command into an array: [Alice, 10]
    char ** values = (char **)malloc(sizeof(char *) * STND_SIZE);
    int i = 0;
    while (cmd) {
      
      // getting individual listed values
      char * t = strsep(&cmd, ",");
      
      /*printf("taken value %s\n", t);
      printf("what is left of cmd %s\n", t);*/
      
      if (strstr(t, "\""))
        t = findStringPair(&t, "\"", "\"");
        
      stripWhiteSpace(&t);
      //printf("putting into array %s\n", t);
      values[i] = t;
      i++;
    }
    values[i] = 0;
    
    printf("values[0]: %s\n", values[0]);
    printf("values[1]: %s\n", values[1]);
    
    struct data_table * tables = (*db).DATATABLES;
    struct data_table * table = (struct data_table *)malloc( sizeof(struct data_table) );
    
    // number of tables
    int n = 0;
    while( (*db).TABLENAMES[n] != NULL ){
      n++;
    }
    //printf("number of tables: %d\n", n);
    
    for(i = 0; i < n; i++) {
      if (strcmp(tableName, tables[i].TABLENAME) == 0) {
        (*table).TABLENAME = tables[i].TABLENAME;
        (*table).HEADERS = tables[i].HEADERS;
        (*table).TYPES = tables[i].TYPES;
        (*table).VALUES = tables[i].VALUES;
        break;
      }
    }
    
    struct data_entry ** deList = (struct data_entry **)malloc (sizeof(struct data_entry *) * 10);
    deList = table->VALUES;
    int row = 0;
    while (deList[row]) {
      row++;
    }
    
    struct data_entry * de = (struct data_entry *)malloc (sizeof(struct data_entry) * 10);
    i = 0;
    while (values[i]) {
      if (strcmp("TEXT", (table->TYPES)[i]) == 0) {
        de[i].TYPE = "TEXT";
        de[i].TEXT_VAL = values[i];
      }
      else if (strcmp("INTEGER", (table->TYPES)[i]) == 0) {
        de[i].TYPE = "INTEGER";
        de[i].INT_VAL = atoi(values[i]);
      }
      i++;
    }
    //de[i] = NULL; 
    
    deList[row] = de;
    deList[row+1] = NULL;
    
    /*printf("%s\n", deList[row][0].TEXT_VAL);
    printf("%s\n", deList[row][1].TEXT_VAL);*/
    table->VALUES = deList;
    writeDatabase(*db, filename);
  }
  
  // .tables command
  else if (strcmp(cmd, ".tables") == 0) {
    struct database * db = readDatabase(filename);
    char ** tables = (*db).TABLENAMES;
    int i = 0;
    while (tables[i]) {
      printf("%s\n", tables[i]);
      i++;
    }
  }
}


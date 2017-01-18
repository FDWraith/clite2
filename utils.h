extern struct database;
extern struct data_table;

char * stripWhiteSpace( char ** originalString );
char * findStringPair( char ** originalString, char * firstDenom, char * secondDenom);
void lock();
void unlock();
void printDatabase( struct database * db, char ** listOfTableNames, int numOfTables );
void printTable( struct data_table * tb );

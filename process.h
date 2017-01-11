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
  int NUM_OF_TABLES;
  struct data_table * DATATABLES;
};


int openFileAttempt(char * filename);
struct database * readDatabase(int fd);
void lock();
void unlock();
char * findStringPair( char ** originalString, char * firstDenom, char * secondDenom);
struct data_table turnStringToTable(char ** fullString, char * tablename);
/*Struct For Storing a Single Entry in the Datatable. Because you are capable of storing different types of items, this is a workaround I have come up with
 *If TYPE == TEXT:
 *  TEXT_VAL is populated
 *Else:
 *  INT_VAL is populated
 */
struct data_entry {
  char * TYPE;
  int INT_VAL;
  char * TEXT_VAL;
};

/*Struct for a single table
 *  TABLENAME : Name of the table. String.
 *  HEADERS : Header for each column of the table. List of Strings
 *  TYPES : Type for each column of the table. List of Strings
 *  VALUES : Tangible data. Organized in rows, with each row containing data_entries. List of List of data_entry 
 */
struct data_table {
  char * TABLENAME;
  char * * HEADERS;
  char * * TYPES;
  struct data_entry * * VALUES;
};


/*Struct for entire database in the file
 *  TABLENAMES : List of tables by their names.
 *  NUM_OF_TABLES (CURRENTLY NOT WORKING) : Number of tables in the database
 *  DATATABLES : List of tables by their structs. 
 */
struct database{
  char * * TABLENAMES;
  int * NUM_OF_TABLES;
  struct data_table * DATATABLES;
};

/*Reads the database, given proper formatting ( see the file DBDenom ),  from the given file
 *@params 
 *  filename : file that is to be opened.
 *@return
 *  returns a database struct with all the necessary information ( see structs ). It is possible that there is data missing, such as in the case of an empty table, so it is wise to check.
 */
struct database * readDatabase(char * filename);

/*Converts a properly formatted string ( see the file DBDenom ) into a struct data_table. Finds the table denomed by the tablename, and modifies the string.
 *@params
 *  fullString : address of the formatted string. The string will be modified as a result, leaving only the part AFTER the denom. 
 *  tablename : name of the table you are looking for. NOTE: prior tables cannot be accessed after using this, so be careful of order.
 *@return
 *  returns a struct data_table
 */
struct data_table turnStringToTable(char ** fullString, char * tablename);

/*Reverses the process done by turnStringToTable.
 *@params
 *  table : the table you are trying to convert
 *@return
 *  returns a properly formatted string ( see the file DBDenom ) that you can pop into a file. 
 */
char * turnTableToString( struct data_table table );

/*Converts the entire database into a properly formatted string ( see the file DBDenom ) and writes it to the file. Note : the file is overwritten. All previous data is lost. 
 *@params
 *  db : the database you are trying to convert
 *  filename : the name of the file you are writing to
 */
void writeDatabase( struct database db, char * filename );

/*Creates a file if it does not exist. Perms are 0644. 
 *@params
 *  filename : name of the file. 
 */
void createFileIfNotExists( char * filename );

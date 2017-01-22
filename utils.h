extern struct database; // See process.h for details 
extern struct data_table; // See process.h for details. 

/*Searches the beginning and end of originalString for whiteSpace characters ( ' ', and '\n' being two examples ) and removes them. The originalString is modified as a result.
 *@params
 *  originalString : address of the string you are trying to modify.
 *@return
 *  returns a copy of the originalString, in case you want to use it for something else. Note : the originalString is still modified. 
 */
char * stripWhiteSpace( char ** originalString );

char * removeChar (char ** originalString, char c);

/*Pairs firstDenom with secondDenom in a given originalString, and returns everything in between. The originalString is modified as a result.
 *@params
 *  originalString : the address of the string you are searching through for.
 *  firstDenom : string of the opening part of what you are looking for.
 *  secondDenom : string of the ending part of what you are looking for.
 *@return
 *  returns the string between the denoms. If not found, it returns an error.
 *Example of use:
 *   char * string = "( > . < ) HAHAHA ";
 *   findStringPair( &string, "(", ")" );
 *      - returns " > . < ";
 *      - string becomes " HAHAHA ";
 */
char * findStringPair( char ** originalString, char * firstDenom, char * secondDenom );

/*Locks. Use whenever you need to access the file.
 */
void lock();

/*Unlocks. Use when you are done using the file.
 */
void unlock();

/*Prints the chosen tables from the database in proper SQlite3 format.
 *@params
 *  db : the entire database
 *  listOfTableNames : List of tables that you are printing out. 
 *  numOfTables : For ease of looping. The number of tables you are printing out.
 *@return
 *  prints out the database.
 */
void printDatabase( struct database * db, char ** listOfTableNames, int numOfTables );

/*Print a single table 
 *@params
 *  tb : the table
 */
void printTable( struct data_table * tb );

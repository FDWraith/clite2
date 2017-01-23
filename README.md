# **CLite2** 

---

### By **Team C\#** : Tina La, Kevin Zhang

**Description:** A C-version of the command-line utility sqlite3. Some of the features of the database editor / creator of sqlite3 have been implemented, but not all of them, as that would be too much work for two students to complete. This database utility is capable of file locking, creating and deleting tables, and selecting data from and inserting data to a table. 

**Instructions**

1. Compile our code by running ```make```

2. Run ```./clite2 <filename>``` It is important that you give a filename, as the program will not run without it. This file will be your database file. The program will create the file if it does not already exist.

3. Once the shell is running, you can run a number of commands

   - Dot Commands ( . )

     - ```.help``` lists all the dot commands, as well as any information regarding those commands

     - ```.tables``` lists all of the tables, by TABLENAME

     - ```.quit``` and ```.exit``` will exit the program.
   - Shell Commands 
     - ```CREATE TABLE``` is a useful command that allows the use to create tables in the database. 
       - Ex. ```CREATE TABLE <tablename> ( <item1> <TYPE1>, <item2> <TYPE2>, ... )```   
         - ```<tablename>``` is the name of the table you would like to assign.
         - ```item1``` is the header you would like to assign to the first item. ```TYPE1``` is the type of element being stored, such as ```TEXT``` or ```INTEGER```. **Note: Those two types are the only types implemented in this beta version of clite2.  
     - ```SELECT``` another useful command that allows the use of accessing information from the database.
       - Ex. ```SELECT * FROM <tablename>``` or ```SELECT <header> FROM <tablename>``` 
         - ​
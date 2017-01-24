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
     - ```.print``` Prints literal string. 
     - ```.quit``` and ```.exit``` will exit the program.
   - Shell Commands 
     - ```CREATE TABLE``` is a useful command that allows the use to create tables in the database. 
       - Ex. ```CREATE TABLE <tablename> ( <item1> <TYPE1>, <item2> <TYPE2>, ... )```   
         - ```<tablename>``` is the name of the table you would like to assign.
         - ```item1``` is the header you would like to assign to the first item. ```TYPE1``` is the type of element being stored, such as ```TEXT``` or ```INTEGER```. *Note: Those two types are the only types implemented in this beta version of clite2.*  
     - ```SELECT``` is another useful command that allows the use of accessing information from the database.
       - Ex. ```SELECT * FROM <tablename>``` or ```SELECT <header> FROM <tablename>``` 
         - ```<tablename>``` is the name of the table you would like to get information from. Getting information from multiple tables is possible.
         - ```<header>``` is the header of the specific information you would like to get from a table. If two tables have the same header, then you have to specify by using ```<tablename>.<header>```
     - ```INSERT INTO``` allows an individual to put information into his/her tables
       - Ex. ```INSERT INTO <tablename> VALUES ( <item1>, <item2>, <item3> ... )```
         - ```<tablename>``` is the table you are trying to modify
         - ```<item1>``` is the item you are trying to put in. Strings need to be surrounded with "", but integers can be left alone. ```NONE``` or ```NULL``` are **not** valid inputs.
     - ```DELETE``` allows a user to delete his tables.
       - Ex. ```DELETE <tablename>```
         - ```<tablename>``` is the name of the table you are trying to delete.
         - *Note: ```DELETE *``` is not a valid command*
4. You're Done! If you would like to see our testing process, please use ```make runtest```. Otherwise, you can exit the program and enjoy your day. 

**Additional Comments**

1. We have not handled all the possible points in which our code may break. Some are handled, for example trying ```SELECT * FROM *```, but others may cause our code to function weirdly. All the commands displayed on this page work as intended, but if you attempt special cases, then things may not work. Please do not intentionally break our code, as the database file may end up corrupt as a result ( unlikely, but still a possibility ).
2. Currently, it is very easy to read the database information directly from the file. This is intentional, as it allows a person to understand whether something worked or not. If we were to expand this project outside of beta, then the file information will likely be encrypted, or something akin to that.
3. ```WHERE```, which is a core feature of sqlite3, has not been implemented here. If we had additional time, we would have done it, but it's a very difficult feature to implement. 
4. Several of sqlite3's ```.help``` menu features are not implemented as well. 


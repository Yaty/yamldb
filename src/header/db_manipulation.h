/*
**  Filename : db_manipulation.h
**
**  Made by : Vincent GUÉNIN ESGI - 3AL-1
**
**  Description : Contains the database manipulation functions prototypes & struct declarations
*/

#ifndef DB_MANIPULATION_H_INCLUDED
#define DB_MANIPULATION_H_INCLUDED

/*================ STRUCT DECLARATION ================*/


/*================ PROTOTYPES ================*/
/*
Goal : Manage the database's manipulations
Input : db (char*), name of the database we manipulate
Output : void
*/
void databaseManipulationManager(char *db);

/*
Goal : display the menu for the database manipulation
Input : void
Output : short, choice of the user in the menu
*/
short databaseManipulationManagerMenu(void);

/*
Goal : Remove all tables' files
Input : dirName (char*), path of the database to destroy
Output : short, state of the process :
            - 0, success
            - 1, error while opening the directory
            - 2, error while memory allocation
            - 3, error while deleting the file
*/
short deleteTableFiles(char *dirName);

/*
Goal : Drop a database
Input : dbName (char*), name of the database to destroy
Output : short, state of the function
            - 0, success
            - 1, error while opening the directory
            - 2, error while memory allocation
            - 3, error while deleting the table's file
            - 4, error while deleting the db's directory
            - 5, error while deleting the db's file
*/
short dropDatabase(char *dbName);

/*
Goal : Manage the db suppression
Input : dbName (char*), name of the database to suppress
Output : short, state of the process
            - 0, success
            - other, error
*/
short dropDatabaseManager(char *dbName);

/*
Goal : Create a table
Input : db (char*), name of the db we manipulate
Output : void
*/
void createTable(char *db);

/*
Goal : Create a table in database folder
Input : db (char*), name of the db we manipulate
        tableName(char*), name of the table
Output : int
*/
int createTableFile(char *db, char *tableName);


#endif // DB_MANIPULATION_H_INCLUDED

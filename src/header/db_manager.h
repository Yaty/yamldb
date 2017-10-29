/*
**  Filename : db_manager.h
**
**  Made by : Vincent GUÉNIN ESGI - 3AL-1
**
**  Description : Contains the database management functions prototypes & struct declarations
*/

#ifndef DB_MANAGER_H_INCLUDED
#define DB_MANAGER_H_INCLUDED

/*================ STRUCT DECLARATION ================*/


/*================ PROTOTYPES ================*/
/*
Goal : Manage the database
Input : void
Output : void
*/
void databaseManager(void);

/*
Goal : display the menu for the database manager
Input : void
Output : char, choice of the user in the menu
*/
char displayDatabaseManagerMenu(void);

/*
Goal : Create a database
Input : name (char*), name of the database to create
Output : void
*/
void createDatabase(char* name);

/*
Goal : Create a database manager
Input : void
Output : void
*/
void createDatabaseManager(void);

/*
Goal : Get the names of all databases created
Input : - dbNamesLength (short*), length of incomeArray (char***)
        - dbNames (char***), array to fill.
Output : char, state of the treatment :
            - 0, success
            - 1, error while opening the directory
            - 2, error while memory allocation
Require : - dbNames (char***) needs to be free.
          - dbNames (char***) needs to be initialized - malloc(0) works
*/
char getDatabasesList(short *dbNamesLength, char ***dbNames);

/*
Goal : Get the names of all databases
Input : - resultArrayLength (short*), length of resultArray.
        - resultArray (char***), at the end of the function, this array contains
            the name of every database.
Output : void
Require : - resultArray char*** needs to be free.
*/
void getDatabasesListManager(short *resultArrayLength, char ***resultArray);

/*
Goal : Display the databases' list
Input : void
Output : void
*/
void displayDatabasesList(void);

#endif // DB_MANAGER_H_INCLUDED

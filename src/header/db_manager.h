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


#endif // DB_MANAGER_H_INCLUDED

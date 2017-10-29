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


#endif // DB_MANIPULATION_H_INCLUDED

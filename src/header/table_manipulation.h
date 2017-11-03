/*
**  Filename : table_manipulation.h
**
**  Made by : Alexis BENARD ESGI - 3AL-1
**
**  Description : Contains the table management functions prototypes & struct declarations
*/

#ifndef TABLE_MANAGER_H_INCLUDED
#define TABLE_MANAGER_H_INCLUDED

/*
Goal : Manage the table's manipulations
Input : - dbName (char*), name of the database we manipulate
        - tableName (char*), name of the table we manipulate
Output : void
*/
void tableManipulationManager(char *dbName, char *tableName);

/*
Goal : display the menu for the table manipulation
Input : - dbName (char*), name of the database
        - tableName (char*), name of the table
Output : short, choice of the user in the menu
*/
short tableManipulationManagerMenu(char *dbName, char *tableName);

/*
Goal : Suppress a table from a database
Input : - dbName (char*), name of the database
        - tableName (char*), name of the table we want to suppress
Input : short, state of the process
            - 0, success
            - 1, error while deleting the table's file
*/
short dropTable(char *dbName, char *tableName);

/*
Goal : Delete the line that correspond to the table to suppress in the db's file
Input : - dbName (char*), name of the database
        - tableName (char*), name of the table to suppress
Output : void
*/
void deleteTableInDb(char *dbName, char *tableName);

#endif // TABLE_MANAGER_H_INCLUDED
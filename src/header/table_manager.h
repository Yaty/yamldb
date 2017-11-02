/*
**  Filename : table_manager.h
**
**  Made by : Alexis BENARD ESGI - 3AL-1
**
**  Description : Contains the table management functions prototypes & struct declarations
*/

#ifndef TABLE_MANAGER_H_INCLUDED
#define TABLE_MANAGER_H_INCLUDED

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

#endif // TABLE_MANAGER_H_INCLUDED

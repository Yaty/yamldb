/*
**  Filename : db_manipulation.h
**
**  Made by : Vincent GU�NIN ESGI - 3AL-1
**
**  Description : Contains the database manipulation functions prototypes & struct declarations
*/

#ifndef DB_MANIPULATION_H_INCLUDED
#include "../header/yaml/api.h"
#define DB_MANIPULATION_H_INCLUDED

/*================ STRUCT DECLARATION ================*/


/*================ PROTOTYPES ================*/
/*
Goal : Manage the database's manipulations
Input : dbName (char*), name of the database we manipulate
Output : void
*/
void databaseManipulationManager(char *dbName);

/*
Goal : display the menu for the database manipulation
Input : dbName (char*), name of the database
Output : short, choice of the user in the menu
*/
short databaseManipulationManagerMenu(char *dbName);

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
Input : db (char*), name of the database we manipulate
Output : void
*/
void createTable(char *db);

/*
 * Goal : Init the created table file with the root node
 * Input : path (char*), path of the created file
 * Output : void
 */
void createTableFileInit(char* path);

/*
Goal : Create a table in database folder
Input : db (char*), name of the db we manipulate
        tableName(char*), name of the table
Output : int
*/
int createTableFile(char *db, char *tableName);

/*
 * Goal : Save the table into the yaml db file
 * Input :  - dbName (char*), name of the database
 *          - tableName (char*), name of the table
 * Output : void
 */
void addTableToDbFile(char* dbName, char* tableName);

/*
Goal : Get the names of all tables created
Input : - dirName (char*), path of the db directory
        - tableNamesLength (short*), length of tableNames (char***)
        - tableNames (char***), array to fill.
Output : char, state of the treatment :
            - 0, success
            - 1, error while opening the directory
            - 2, error while memory allocation
Require : - tableNames (char***) needs to be free.
          - tableNames (char***) needs to be initialized - malloc(0) works
*/
char getTablesList(char *dirName, short *tableNamesLength, char ***tableNames);

/*
Goal : Get the names of all tables
Input : - dirName (char*), path of the database's directory
        - resultArrayLength (short*), length of resultArray.
        - resultArray (char***), at the end of the function, this array contains
            the name of every database.
Output : void
Require : - resultArray char*** needs to be free.
*/
void getTablesListManager(char *dirName, short *resultArrayLength, char ***resultArray);

/*
Goal : Display the tables' list
Input : dirName (char*), path of the database's directory
Output : void
*/
void displayTablesList(char *dirName);

/*
Goal : Manage the tables' list display
Input : dirName (char*), path of the database's directory
Output : void
*/
void displayTablesListManager(char *dirName);

/*
Goal : Manage the table opening
Input : void
Output : void
*/
void openTableManager(char *dbName);

/*
Goal : Ask the table to open
Input : - length (short), length of array
        - array (char**), list of the tables' name
Output : number of the table
*/
short openTableAskNumber(short length, char **array);

/*
 * Goal : Create a Database and the associated file
 * Input : void
 * Output : void
 */
void testCreateDb();

/*
 * Goal : Add a table to a database
 * Input : void
 * Output : void
 * */
void testAddTable();

/*
 * Goal : Add attributs to columns line.
 * Input : str (char *)
 * Output : void
 * */
void createAttribut(char *str);

/*
 * Goal : defined attribut to column.
 * Input : dbName (char *)
 *         table (char *)
 * Output : void
 * */
void definedAttributs(char *dbName, char *table);

/*
 * Goal : defined the primary key.
 * Input : dbName (char *)
 *         table (char *)
 * Output : void
 * */
void addCaracteristics(char *dbName, char *table, int type);

/*
 * Goal : display columns from yaml file.
 * Input : colums (Node *)
 * Output : choice of column
 * */
int displayColumns(Node *columns);

/*
 * Goal : set values for columns caracteristics.
 * Input : result (Node *)
 *         choice (int)
 *         text (*char)
 *         path (*char)
 *         type (int)
 * Output : void
 * */
void setCaracteristics(Node *result, int choice, char *text, char *path, int type);
#endif // DB_MANIPULATION_H_INCLUDED

#ifndef SQL_MODULE_H_INCLUDED
#define SQL_MODULE_H_INCLUDED

/* =============== STRUCTURES =============== */

/* =============== PROTOTYPES =============== */

/*
Goal : place the sql module
Input : - grid (GtkWidget*), grid of the window
        - dbName (char**), name of the active database
Output : void
*/
void displaySqlModule(GtkWidget *grid, char **dbName);

/*
Goal : Execute a SQL query and parse the result in the Result Module
Input : - button (GtkWidget*), button we clicked to access this function
        - data (gpointer), data to transfer to the function
Output : void
*/
void executeSql(GtkWidget *button, gpointer data);


#endif // SQL_MODULE_H_INCLUDED

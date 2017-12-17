/*
**  Filename : window_manager.h
**
**  Made by : Vincent GUENIN ESGI - 3AL-1
**
**  Description : Contains the gtk windows functions prototypes
*/
#ifndef WINDOW_MANAGER_H_INCLUDED
#define WINDOW_MANAGER_H_INCLUDED


#include "../../header/sql/query.h"

/* =============== STRUCTURES =============== */



/* =============== PROTOTYPES =============== */
/*
Goal : Initialize gtk and
Input : argc, argv
Output : int, status of program
*/
int guiMain(int argc, char **argv);

/*
Goal : Create a window
Input : - window (GtkWidget**), window to return
        - default_width (gint), width of the window
        - default_height (gint), height of the window
Output: void
*/
void createMainWindow(GtkWidget **window, gint default_width, gint default_height);

/*
Goal : Manage the process of the program
Input : void
Output : void
*/
void guiManager();

/*
Goal : Place objects in the window
Input : window (GtkWidget*), window of the program
Output : void
*/
void placeWidgets(GtkWidget *window);

/*
Goal : place the sql result module
Input : grid (GtkWidget*), grid of the window
Output : void
*/
void displayResultModule(GtkWidget *grid);

/*
Goal : place table display module
Input : grid (GtkWidget*), grid of the window
Output : void
*/
void displayTableModule(GtkWidget *grid);

/*
Goal : Change the global dbPath
Input : - button (GtkWidget*), button we clicked to access this function
        - data (gpointer), data to transfer to the function
Output : void
*/
void dbClicked(GtkWidget *button, gpointer data);

/*
Goal : place the sql module
Input : grid (GtkWidget*), grid of the window
Output : void
*/
void displaySqlModule(GtkWidget *grid);

/*
Goal : Execute a SQL query and parse the result in the Result Module
Input : - button (GtkWidget*), button we clicked to access this function
        - data (gpointer), data to transfer to the function
Output : void
*/
void executeSql(GtkWidget *button, gpointer data);

/*
Goal : Display the result of the query
Input : result (QueryResult*), result of the sql query
Output : void
*/
void displaySqlQueryResult(QueryResult *result);

/*
Goal : Change the text in the result label
Input : result (QueryResult*), result of the sql query
Output : void
*/
void changeResultLabelText(QueryResult *result);

#endif // WINDOW_MANAGER_H_INCLUDED

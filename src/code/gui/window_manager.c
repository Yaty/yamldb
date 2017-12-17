/*
**  Filename : window_manager.c
**
**  Made by : Vincent GUENIN ESGI - 3AL-1
**
**  Description : Contains the gtk windows functions
*/
/*================ INCLUDES ================*/
#include <gtk/gtk.h>
#include <glib.h>
#include <string.h>
#include "../../header/gui/window_manager.h"
#include "../../header/db_manager.h"
//#include "../../header/gui/sql_module.h"

/* ================= GLOBAL ================= */
GtkWidget *window;
GtkWidget *resultGrid = NULL;
GtkWidget *sqlModuleTitleLabel = NULL;
GtkWidget *resultLabel = NULL;
char dbPath[255] = "";

/*
Goal : Initialize gtk and
Input : argc, argv
Output : int, status of program
*/
int guiMain(int argc, char **argv) {
    gtk_init (&argc, &argv);
    guiManager();
    gtk_main();

    return 0;
}

/*
Goal : Create a window
Input : - window (GtkWidget**), window to return
        - default_width (gint), width of the window
        - default_height (gint), height of the window
Output: void
*/
void createMainWindow(GtkWidget **window, gint default_width, gint default_height){
    /* Création de la fenetre*/
    *window = gtk_window_new(GTK_WINDOW_TOPLEVEL); // création de la fenetre
    gtk_window_set_title(GTK_WINDOW (*window), "CBDD1"); // titre de la fenetre
    gtk_window_set_default_size(GTK_WINDOW(*window), default_width, default_height); // taille par défaut de la fenetre
    gtk_window_set_position (GTK_WINDOW (*window), GTK_WIN_POS_CENTER_ALWAYS); // Positionnement de la fenetre
    g_signal_connect(*window, "destroy", G_CALLBACK(gtk_main_quit), NULL); //appelle de la fonction destroy pour sortir de la boucle infinie
}

/*
Goal : Manage the process of the program
Input : void
Output : void
*/
void guiManager() {
    createMainWindow(&window, 1000, 600);

    placeWidgets(window);

    gtk_widget_show_all(window); //Affiche les widgets
}

/*
Goal : Place objects in the window
Input : window (GtkWidget*), window of the program
Output : void
*/
void placeWidgets(GtkWidget *window) {
    GtkWidget *grid;

    //Create the grid
    grid = gtk_grid_new();
    gtk_grid_set_row_spacing (GTK_GRID(grid), 10);
    gtk_container_set_border_width (GTK_CONTAINER(window), 5);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);

    //Add the grid to the window
    gtk_container_add(GTK_CONTAINER(window), grid);

    //Add modules
    displayTableModule(grid);
    displaySqlModule(grid);
    displayResultModule(grid);
}

/*
Goal : place the sql result module
Input : grid (GtkWidget*), grid of the window
Output : void
*/
void displayResultModule(GtkWidget *grid) {
    GtkWidget *titleLabel = gtk_label_new("SQL result Module");
    resultLabel = gtk_label_new("");

    //Initialize the grids
    GtkWidget *grid2 = gtk_grid_new();
    gtk_grid_set_row_spacing (GTK_GRID(grid2), 10);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid2), FALSE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid2), FALSE);

    resultGrid = gtk_grid_new();
    gtk_grid_set_row_spacing (GTK_GRID(resultGrid), 10);
    gtk_grid_set_column_homogeneous(GTK_GRID(resultGrid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(resultGrid), FALSE);

    //Attach the grids
    gtk_grid_attach(GTK_GRID(resultGrid), titleLabel, 0, 0, 1, 1); //Attach to grid2
    gtk_grid_attach(GTK_GRID(resultGrid), resultLabel, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(resultGrid), grid2, 0, 2, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), resultGrid, 1, 1, 3, 2); //Attach grid2 to main grid
}

/*
Goal : place table display module
Input : grid (GtkWidget*), grid of the window
Output : void
*/
void displayTableModule(GtkWidget *grid) {
    GtkWidget *tableGrid = gtk_grid_new();

    //Initialize grid
    gtk_grid_set_row_spacing (GTK_GRID(tableGrid), 10);
    gtk_grid_set_column_homogeneous(GTK_GRID(tableGrid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(tableGrid), FALSE);

    //Set title label
    GtkWidget *title_label = gtk_label_new("Bases de donnees");
    gtk_grid_attach(GTK_GRID(tableGrid), title_label, 0, 0, 1, 1);

    //Get database list
    short dbListLength = 0;
    char **dbList;
    getDatabasesList(&dbListLength, &dbList);

    //Display database list
    GtkWidget *button;
    for( int counter = 0; counter < dbListLength; counter++ ) {
        button = gtk_button_new_with_label(dbList[counter]);
        gtk_grid_attach(GTK_GRID(tableGrid), button, 0, counter + 1, 1, 1);
        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(dbClicked), dbList[counter]);
    }

    gtk_grid_attach(GTK_GRID(grid), tableGrid, 0, 0, 1, 3);
}

/*
Goal : Change the global dbPath
Input : - button (GtkWidget*), button we clicked to access this function
        - data (gpointer), data to transfer to the function
Output : void
*/
void dbClicked(GtkWidget *button, gpointer data) {
    char *name = data;

    //Change active database Path
    strcpy(dbPath, "./resources/");
    strcat(dbPath, name);
    //strcat(dbPath, "/");

    //Change active database displays
    gchar sqlModuleTitle[150] = "SQL Module - Base active : ";
    strcat(sqlModuleTitle, name);
    gtk_label_set_text(GTK_LABEL(sqlModuleTitleLabel), sqlModuleTitle);
}

/*
Goal : place the sql module
Input : grid (GtkWidget*), grid of the window
Output : void
*/
void displaySqlModule(GtkWidget *grid) {
    GtkWidget *sqlGrid = gtk_grid_new();

    //Initialize grid
    gtk_grid_set_row_spacing (GTK_GRID(sqlGrid), 10);
    gtk_grid_set_column_homogeneous(GTK_GRID(sqlGrid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(sqlGrid), TRUE);

    //Get active database name
    gchar title[255] = "SQL Module - Base active : ";
    strcat(title, "Aucune");

    //Set the title label
    sqlModuleTitleLabel = gtk_label_new(title);
    gtk_grid_attach(GTK_GRID(sqlGrid), sqlModuleTitleLabel, 0, 0, 10, 1);

    //Set the text view
    GtkWidget *textView = gtk_text_view_new();
    GtkWidget *textBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));
    gchar *text = "SELECT * FROM products"; //TEST

    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(textBuffer), text, (gint)strlen(text));
    gtk_grid_attach(GTK_GRID(sqlGrid), textView, 1, 1, 8, 2);

    //Set the send button
    GtkWidget *button = gtk_button_new_with_label("Executer");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(executeSql), textBuffer);
    gtk_grid_attach(GTK_GRID(sqlGrid), button, 7, 3, 2, 1);

    //Put the sqlGrid into grid
    gtk_grid_attach(GTK_GRID(grid), sqlGrid, 1, 0, 3, 1);
}

/*
Goal : Execute a SQL query and parse the result in the Result Module
Input : - button (GtkWidget*), button we clicked to access this function
        - data (gpointer), data to transfer to the function
Output : void
*/
void executeSql(GtkWidget *button, gpointer data) {
    GtkWidget *textBuffer = data;
    GtkTextIter startIter, endIter;

    //Get the text in the TextView
    gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(textBuffer), &startIter, &endIter);
    gchar *sqlQuery = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(textBuffer), &startIter, &endIter, TRUE);

    //Execute the query
    printf("SQLExecuteQuery('%s', '%s');\n", sqlQuery, dbPath);
    QueryResult *result = SQLExecuteQuery(sqlQuery, dbPath);

    SQLPrintQueryResult(result); //TEST

    displaySqlQueryResult(result);
}

/*
Goal : Display the result of the query
Input : result (QueryResult*), result of the sql query
Output : void
*/
void displaySqlQueryResult(QueryResult *result) {
    //Change the result label content
    changeResultLabelText(result);

    //Change grid contains

}

/*
Goal : Change the text in the result label
Input : result (QueryResult*), result of the sql query
Output : void
*/
void changeResultLabelText(QueryResult *result) {
    int counter;
    char text[1000];
    int warningNumber = result->warningsCounter;

    //Concatenate warnings
    if( warningNumber != 0 ) {
        strcpy(text, "Warning : ");
        strcat(text, result->warnings[0]);
        for( counter = 1; counter < warningNumber; counter++ ) {
            strcat(text, "\nWarning : ");
            strcat(text, result->warnings[counter]);
        }
    }

    //Change resultLabel text
    gtk_label_set_text(resultLabel, text);
}

void displayResult(QueryResult *result) {
    //Initialize new grid
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing (GTK_GRID(grid), 10);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), FALSE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), FALSE);

    //Attach grid to the bigger one
    gtk_grid_attach(GTK_GRID(resultGrid), grid, 0, 2, 1, 1);

    //Fill the grid
    int rows = result->rowsCounter;
    int cols = result->columnsCounter;

    //Set header
    int counter;
    GtkWidget *label;
    for( counter = 0; counter < cols; counter++ ) {
        label = gtk_label_new(result->headers[counter]);
        gtk_grid_attach(GTK_GRID(grid), label, counter, 0, 1, 1);
    }

    //Set content
    int counter2;
    for( counter = 0; counter < rows; counter++ ) {
        for( counter2 = 0; counter2 < cols; counter2++ ) {
            label = gtk_label_new(result->table[counter][counter2]);
            gtk_grid_attach(GTK_GRID(grid), label, counter2, counter + 1, 1, 1);
        }
    }
}

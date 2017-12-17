#include <gtk/gtk.h>
#include <string.h>
#include "../../header/gui/table_module.h"
#include "../../header/db_manager.h"
#include "../../header/gui/window_manager.h"

char **name;

/*
Goal : place table display module
Input : - grid (GtkWidget*), grid of the window
        - dbName (char**), name of the active database
Output : void
*/
void displayTableModule(GtkWidget *grid, char **dbName) {
    name = dbName; //Set the global dbName

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
    char *dbName = data;
    strcpy(*name, dbName);
}

#include <gtk/gtk.h>
#include <string.h>
#include "../../header/gui/sql_module.h"
#include "../../header/sql/query.h"

/*
Goal : place the sql module
Input : - grid (GtkWidget*), grid of the window
        - dbName (char**), name of the active database
Output : void
*/
void displaySqlModule(GtkWidget *grid, char **dbName) {
    char *databaseName = *dbName;
    GtkWidget *sqlGrid = gtk_grid_new();

    //Initialize grid
    gtk_grid_set_row_spacing (GTK_GRID(sqlGrid), 10);
    gtk_grid_set_column_homogeneous(GTK_GRID(sqlGrid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(sqlGrid), TRUE);

    //Get active database name
    gchar title[255] = "SQL Module - Base active : ";
    if( strlen(databaseName) != 0 ) {
        strcat(title, databaseName);
    }else{
        strcat(title, "Aucune");
    }

    //Set the title label
    GtkWidget *label = gtk_label_new(title);
    gtk_grid_attach(GTK_GRID(sqlGrid), label, 0, 0, 10, 1);

    //Set the text view
    GtkWidget *textView = gtk_text_view_new();
    GtkWidget *textBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));
    gchar *text = "Texte dans le textBuffer";

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
    //QueryResult *result = SQLExecuteQuery();
}

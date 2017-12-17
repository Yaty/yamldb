#ifndef TABLE_MODULE_H_INCLUDED
#define TABLE_MODULE_H_INCLUDED

/*
Goal : place table display module
Input : - grid (GtkWidget*), grid of the window
        - dbName (char**), name of the active database
Output : void
*/
void displayTableModule(GtkWidget *grid, char **dbName);

/*
Goal : Change the global dbPath
Input : - button (GtkWidget*), button we clicked to access this function
        - data (gpointer), data to transfer to the function
Output : void
*/
void dbClicked(GtkWidget *button, gpointer data);


#endif // TABLE_MODULE_H_INCLUDED

/*
**  Filename : main.c
**
**  Made by : Hugo DA ROIT ESGI - 3AL-1
**
**  Description : main of the project
**
**  Goal of the project : Create a database manager
**      with a SQL interpreter and a GTK graphical interface
*/

#include <stdlib.h>
#include "../header/db_manager.h"

int main (int argc, char **argv) {
    // PART 1
    databaseManager();

    // PART 2 + 3
    // QueryResult *res = SQLExecuteQuery("SELECT * FROM customers", "./resources/base1");
    // SQLPrintQueryResult(res);
    return EXIT_SUCCESS;
}


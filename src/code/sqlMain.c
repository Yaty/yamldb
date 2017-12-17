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

#include <stdio.h>
#include <stdlib.h>
#include "../header/sql/query.h"

int main (int argc, char **argv) {
    char *buffer;
    size_t bufsize = 2048;
    buffer = (char *)malloc(bufsize * sizeof(char));
    if (buffer == NULL) {
        perror("Unable to allocate buffer");
        exit(1);
    }

    while (1) {
        printf("Please enter a SQL query :\n");
        getline(&buffer, &bufsize, stdin);
        QueryResult *res = SQLExecuteQuery(buffer, "./resources/base1");
        SQLPrintQueryResult(res);
    }

    return EXIT_SUCCESS;
}


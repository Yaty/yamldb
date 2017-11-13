//
// Created by Hugo on 12/11/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../header/string_array_functions.h"
#include "../../header/sql/utils.h"

static char **getColumns(char *query, int *columnsCounter) {
    char *queryCpy = trim(toUpperCase(strdup(query)));

    if (queryCpy) {
        size_t fromIndex = getSubstringIndex(queryCpy, "FROM") - 1;
        queryCpy[fromIndex] = '\0';
        char **columns = getParams(queryCpy, columnsCounter); // Get columns after select
        queryCpy[fromIndex] = ' '; // reset
        free(queryCpy);
        return columns;
    }

    return NULL;
}

static char **getTables(char *query, int *tablesCounter) {
    char *queryCpy = trim(toUpperCase(strdup(query)));

    if (queryCpy) {
        char *queryCpy2 = trim(queryCpy + getSubstringIndex(queryCpy, "FROM") + 5);
        return getParams(queryCpy2, tablesCounter);
    }

    return NULL;
}

static char *parseSelectQuery(char *query, char ***columns, int *columnsCounter, char ***tables, int *tablesCounter) {
    if (strlen(query) >= 8) { // c FROM d, the least chars to have a valid query
        int i = 0;
        *columns = getColumns(query, columnsCounter);
        if (*columnsCounter > 0 && *columns != NULL) {
            *tables = getTables(query, tablesCounter);
            if (*tablesCounter > 0 && *tables != NULL) {
                return NULL; // success
            } else {
                for (i = 0; i < *columnsCounter; i++) free(*columns[i]);
                return "Error 8 : invalid query, please add one or more tables.";
            }
        } else {
            return "Error 9 : invalid query, please add one or more columns to select.";
        }
    } else {
        return "Error 11 : invalid query, please respect the given schema in the documentation.";
    }
}

/**
 * Execute a SQL select from a string which is the query
 * @param query  a string
 * @return a filled query result
 */
QueryResult executeSelect(char *query) {
    int i = 0;
    int columnsCounter = 0;
    int tablesCounter = 0;
    char **columns;
    char **tables;
    char *parseQuery = parseSelectQuery(query, &columns, &columnsCounter, &tables, &tablesCounter);
    if (parseQuery == NULL && columnsCounter > 0 && tablesCounter > 0) {
        QueryResult res = getEmptyResult();

        // TODO : Execute select here, add where clause, order by too
        for (i = 0; i < columnsCounter; i++) printf("Column : %s\n", columns[i]);
        for (i = 0; i < tablesCounter; i++) printf("Table : %s\n", tables[i]);

        for (i = 0; i < columnsCounter; i++) free(columns[i]);
        for (i = 0; i < tablesCounter; i++) free(tables[i]);
        return res;
    } else {
        return getFailedResult("Error 14 : bad query.");
    }
}
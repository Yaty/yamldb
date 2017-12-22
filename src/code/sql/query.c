//
// Created by Hugo on 11/11/2017.
//

#include <string.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/time.h>
#include <stdio.h>
#include "../../header/sql/utils.h"
#include "../../header/string_array_functions.h"
#include "../../header/sql/select.h"
#include "../../header/sql/insert.h"
#include "../../header/sql/delete.h"
#include "../../header/sql/update.h"

/**
 * Execute a sql query on a db
 * @param queryString the query in sql
 * @param dbPath path to the db yaml file
 * @return a query result
 */
QueryResult *SQLExecuteQuery(char *queryString, char *dbPath) {
    if (queryString && dbPath) {
        char *queryCpy = trim(strdup(queryString));
        if (queryCpy) {
            QueryResult *res = getEmptyResult();

            if (startsWith(queryCpy, "select", 1)) {
                res->type = SELECT;
                executeSelect(res, queryCpy + 7, dbPath);
            } else if (startsWith(queryCpy, "insert into", 1)) {
                res->type = INSERT;
                executeInsert(res, queryCpy + 12, dbPath);
            } else if (startsWith(queryCpy, "delete from", 1)) {
                res->type = DELETE;
                executeDelete(res, queryCpy + 12, dbPath);
            } else if (startsWith(queryCpy, "update", 1)) {
                res->type = UPDATE;
                executeUpdate(res, queryCpy + 7, dbPath);
            } else {
                res = getFailedResult("Error 2 : Invalid query. Please use a valid keyword like select, insert, delete ...");
            }

            free(queryCpy);
            return res;
         } else {
             return getFailedResult("Error 3 : error while executing query.");
         }
     }

    return getFailedResult("Error 1 : non-existent query.");
}

void SQLFreeQueryResult(QueryResult *res) {
    int i;
    int j;

    for (i = 0; i < res->rowsCounter; i++) {
        for (j = 0; j < res->columnsCounter; j++) {
            free(res->table[i][j]);
            res->table[i][j] = NULL;
        }

        free(res->table[i]);
        res->table[i] = NULL;
    }

    if (res->table) {
        free(res->table);
        res->table = NULL;
    }

    for (j = 0; j < res->messagesCounter; j++) {
        free(res->messages[j]);
        res->messages[j] = NULL;
    }

    for (j = 0; j < res->warningsCounter; j++) {
        free(res->warnings[j]);
        res->warnings[j] = NULL;
    }

    for (j = 0; j < res->columnsCounter; j++) {
        free(res->headers[j]);
        res->headers[j] = NULL;
    }
}


/**
 * Get initialized QueryResult
 * @return a queryresult
 */
QueryResult *getEmptyResult() {
    QueryResult *res = malloc(sizeof(QueryResult));
    res->rowsCounter = 0;
    res->columnsCounter = 0;
    res->table = NULL;
    res->headers = NULL;
    res->warnings = NULL;
    res->warningsCounter = 0;
    res->messagesCounter = 0;
    return res;
}

/**
 * Get a query result with a failure status and a message
 * @param message
 * @return a query result
 */
QueryResult *getFailedResult(char *message) {
    QueryResult *res = getEmptyResult();
    res->status = FAILURE;
    res->messages = malloc(sizeof(char*));
    res->messages[0] = message;
    return res;
}

static char **getColumnsSizeModifiers(QueryResult *res) {
    if (res && res->columnsCounter > 0) {
        int i;
        int j;
        size_t columnMaxSize;
        size_t resLength;
        size_t columnLength;
        char **columnsSize = malloc(res->columnsCounter * sizeof(char*));
        char *columnSize;

        for (i = 0; i < res->columnsCounter; i++) {
            columnMaxSize = 0;
            columnSize = malloc(sizeof(char) * 32);

            for (j = 0; j < res->rowsCounter; j++) {
                columnLength = strlen(res->table[j][i]);
                if (columnLength > columnMaxSize) {
                    columnMaxSize = columnLength;
                }
            }

            resLength = strlen(res->headers[i]);
            if (resLength > columnMaxSize) {
                columnMaxSize = resLength;
            }

            sprintf(columnSize, "%d", (int) columnMaxSize + 2); // + 1 to have a padding
            columnsSize[i] = concat(3, "%", columnSize, "s |");
            free(columnSize);
        }

        return columnsSize;
    }

    return NULL;
}


/**
 * Print a query result
 */
void SQLPrintQueryResult(QueryResult *res) {
    int i;
    int j;
    long colSize;

    if (res && res->rowsCounter > 0) {

        char **columnsSizeModifiers = getColumnsSizeModifiers(res);
        char *columnSize;

        // HEADER BAR
        for (i = 0; i < res->columnsCounter; i++) {
            if (i == 0) printf("|");
            columnSize = malloc(sizeof(char) * strlen(columnsSizeModifiers[i]) - 1);
            substring(columnsSizeModifiers[i], columnSize, 1, strlen(columnsSizeModifiers[i]) - 1); // retrieve column length in the modifier
            for (j = 0, colSize = strtol(columnSize, NULL, 10); j <= colSize; j++) {
                printf("-");
            }
            printf("|");
            free(columnSize);
        }
        printf("\n");

        // HEADERS
        for (i = 0; i < res->columnsCounter; i++) {
            if (i == 0) printf("|");
            printf(columnsSizeModifiers[i], res->headers[i]);
        }
        printf("\n");

        for (i = 0; i < res->columnsCounter; i++) {
            if (i == 0) printf("|");
            columnSize = malloc(sizeof(char) * strlen(columnsSizeModifiers[i]) - 1);
            substring(columnsSizeModifiers[i], columnSize, 1, strlen(columnsSizeModifiers[i]) - 1); // retrieve column length in the modifier
            for (j = 0, colSize = strtol(columnSize, NULL, 10); j <= colSize; j++) {
                printf("-");
            }
            printf("|");
            free(columnSize);
        }
        printf("\n");

        // LINES
        for (i = 0; i < res->rowsCounter; i++) {
            printf("|");
            for (j = 0; j < res->columnsCounter; j++) {
                printf(columnsSizeModifiers[j], res->table[i][j]);
            }
            printf("\n");
        }

        // FOOTER BAR
        for (i = 0; i < res->columnsCounter; i++) {
            if (i == 0) printf("|");
            columnSize = malloc(sizeof(char) * strlen(columnsSizeModifiers[i]) - 1);
            substring(columnsSizeModifiers[i], columnSize, 1, strlen(columnsSizeModifiers[i]) - 1); // retrieve column length in the modifier
            for (j = 0, colSize = strtol(columnSize, NULL, 10); j <= colSize; j++) {
                printf("-");
            }
            printf("|");
            free(columnSize);
        }
        printf("\n");

        for (i = 0; i < res->columnsCounter; i++) {
            free(columnsSizeModifiers[i]);
        }
    } else if (res->type == SELECT) {
        printf("No results.\n");
    }

    // WARNINGS
    for (i = 0; i < res->warningsCounter; i++) {
        printf("WARNING : %s\n", res->warnings[i]);
    }

    // MESSAGE
   for (i = 0; i < res->messagesCounter; i++) {
       printf("%s\n", res->messages[i]);
   }
}


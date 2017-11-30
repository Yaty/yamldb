//
// Created by Hugo on 11/11/2017.
//

#include <string.h>
#include <stdlib.h>
#include <sched.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include "../../header/sql/utils.h"
#include "../../header/string_array_functions.h"
#include "../../header/sql/select.h"
#include "../../header/sql/insert.h"

/**
 * Execute a sql query on a db
 * @param queryString the query in sql
 * @param dbPath path to the db yaml file
 * @return a query result
 */
QueryResult SQLExecuteQuery(char *queryString, char *dbPath) {
    if (queryString && dbPath) {
        char *queryCpy = toLowerCase(trim(strdup(queryString)));
        if (queryCpy) {
            QueryResult res;
            short prependIndex = 0;
            char timeSpent[32];
            struct timeval start, end;
            gettimeofday(&start, NULL);

            if (startsWith(queryCpy, "select")) {
                prependIndex = 7; // length of "select "
                res = executeSelect(queryCpy + prependIndex, dbPath);
            } else if (startsWith(queryCpy, "insert")) {
                prependIndex = 7;
                res = executeInsert(queryCpy + prependIndex, dbPath);
            } else {
                res = getFailedResult("Error 2 : Invalid query. Please use a valid keyword like select, insert ...");
            }

            gettimeofday(&end, NULL);
            sprintf(timeSpent, "%ld", (end.tv_usec - start.tv_usec) / 1000);
            res.message = res.status == SUCCESS
                ? concat(3, "Successfully executed query in ", timeSpent, "ms.")
                : concat(3, "Failure while executing the query in ", timeSpent, "ms.");
            free(queryCpy - prependIndex);
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

    if (res->message) {
        free(res->message);
        res->message = NULL;
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
QueryResult getEmptyResult() {
    QueryResult res;
    res.rowsCounter = 0;
    res.columnsCounter = 0;
    res.table = NULL;
    res.headers = NULL;
    res.warnings = NULL;
    res.warningsCounter = 0;
    return res;
}

/**
 * Get a query result with a failure status and a message
 * @param message
 * @return a query result
 */
QueryResult getFailedResult(char *message) {
    QueryResult res = getEmptyResult();
    res.status = FAILURE;
    res.message = message;
    return res;
}


/**
 * Print a query result
 */
void printQueryResult(QueryResult *res) {
    int i;
    int j;

    // HEADERS
    for (i = 0; i < res->columnsCounter; i++) {
        if (i == 0) printf("|");
        printf("%10s|", res->headers[i]);
    }
    printf("\n");

    for (i = 0; i < res->columnsCounter; i++) {
        if (i == 0) printf("|");
        printf("==========|");
    }
    printf("\n");

    // LINES
    for (i = 0; i < res->rowsCounter; i++) {
        printf("|");
        for (j = 0; j < res->columnsCounter; j++) {
            printf("%10s|", res->table[i][j]);
        }
        printf("\n");
    }

    // WARNINGS
    for (i = 0; i < res->warningsCounter; i++) {
        printf("WARN : %s\n", res->warnings[i]);
    }

    // MESSAGE
    printf("%s\n", res->message);
}


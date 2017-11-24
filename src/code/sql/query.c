//
// Created by Hugo on 11/11/2017.
//

#include <string.h>
#include <stdlib.h>
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
     if (queryString) {
         char *queryCpy = toLowerCase(trim(strdup(queryString)));
         if (queryCpy) {
             QueryResult res;
             short prependIndex = 0;

             if (startsWith(queryCpy, "select")) {
                 prependIndex = 7; // length of "select "
                 res = executeSelect(queryCpy + prependIndex, dbPath);
             } else if (startsWith(queryCpy, "insert")) {
                 prependIndex = 7;
                 res = executeInsert(queryCpy + prependIndex, dbPath);
             } else {
                 res = getFailedResult("Error 2 : Invalid query. Please use a valid keyword like select, insert ...");
             }

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

    free(res);
    res = NULL;
}


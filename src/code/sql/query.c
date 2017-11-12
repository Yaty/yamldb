//
// Created by Hugo on 11/11/2017.
//

#include <string.h>
#include <stdlib.h>
#include "../../header/sql/utils.h"
#include "../../header/string_array_functions.h"
#include "../../header/sql/select.h"
#include "../../header/sql/insert.h"

QueryResult SQLExecuteQuery(char *queryString) {
     if (queryString) {
         char *queryCpy = toUpperCase(trim(strdup(queryString)));
         if (queryCpy) {
             QueryResult res;

             if (startsWith(queryCpy, "SELECT")) {
                 res = executeSelect(queryCpy + 7); // 7 is the length of SELECT + 1
             } else if (startsWith(queryCpy, "INSERT")) {
                 res = executeInsert(queryCpy + 7);
             } else {
                 res = getFailedResult("Error 2 : Invalid query. Please use a valid keyword like SELECT, INSERT ...");
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

    for (i = 0; i < res->linesCounter; i++) {
        for (j = 0; j < res->rowsCounter; j++) {
            free(res->lines[i][j]);
            res->lines[i][j] = NULL;
        }

        free(res->lines[i]);
        res->lines[i] = NULL;
    }

    if (res->lines) {
        free(res->lines);
        res->lines = NULL;
    }

    free(res);
    res = NULL;
}


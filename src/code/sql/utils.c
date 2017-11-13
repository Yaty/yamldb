//
// Created by Hugo on 12/11/2017.
//

#include <string.h>
#include <stdlib.h>
#include "../../header/sql/structs.h"
#include "../../header/string_array_functions.h"

/**
 * Get initialized QueryResult
 * @return a queryresult
 */
QueryResult getEmptyResult() {
    QueryResult res;
    res.linesCounter = 0;
    res.rowsCounter = 0;
    res.lines = NULL;
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
 * Get params from a string
 * Example : "param1, param2, param3 bla blabla bla" will return "param1", "param2" and "param3"
 * @param query the string
 * @param paramsCounter the number of params found
 * @return an array of string to free
 */
char **getParams(char *query, int *paramsCounter) {
    if (query && paramsCounter) {
        char *queryCpy = strdup(query);
        char *pt = strtok(queryCpy, ",");
        char **params = NULL;
        char **temp = NULL;
        int i = 0;
        *paramsCounter = 0;

        if (pt == NULL) {
            free(queryCpy);
            return NULL;
        }

        while (pt != NULL) {
            temp = realloc(params, sizeof(char*) * (*paramsCounter + 1));
            if (temp) {
                params = temp;
                char *trimmedParam = trim(strdup(pt));
                params[(*paramsCounter)++] = trimmedParam;
                pt = strtok (NULL, ",");
            } else {
                free(queryCpy);
                for (i = 0; i < *paramsCounter; i++) free(params[i]);
                return NULL;
            }
        }

        return params;
    }

    return NULL;
}
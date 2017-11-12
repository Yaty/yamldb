//
// Created by Hugo on 12/11/2017.
//

#include <stdlib.h>
#include <string.h>
#include "../../header/sql/structs.h"
#include "../../header/sql/utils.h"

/**
 * Execute a insert SQL query
 * @param query
 * @return a query result
 */
QueryResult executeInsert(char *query) {
    QueryResult res = getEmptyResult();
    res.lines = malloc(sizeof(char **));
    res.lines[0] = malloc(sizeof(char *));
    res.lines[0][0] = strdup("BAAAM");
    return res;
}
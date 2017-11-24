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
 * @param dbPath path to the db yaml file
 * @return a query result
 */
QueryResult executeInsert(char *query, char *dbPath) {
    QueryResult res = getEmptyResult();
    res.table = malloc(sizeof(char **));
    res.table[0] = malloc(sizeof(char *));
    res.table[0][0] = strdup("BAAAM");
    return res;
}
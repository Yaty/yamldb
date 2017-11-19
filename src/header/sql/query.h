//
// Created by Hugo on 11/11/2017.
//

#ifndef CBDD1_QUERY_H
#define CBDD1_QUERY_H

#include "structs.h"

/**
 * Execute a sql query on a db
 * @param queryString the query in sql
 * @param dbPath path to the db yaml file
 * @return a query result
 */
QueryResult SQLExecuteQuery(char *queryString, char *dbPath);

void SQLFreeQueryResult(QueryResult *res);

#endif //CBDD1_QUERY_H

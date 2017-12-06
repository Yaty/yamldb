//
// Created by Hugo on 12/11/2017.
//

#ifndef CBDD1_SELECT_H
#define CBDD1_SELECT_H

#include "query.h"

/**
 * Execute a SQL select from a string which is the query
 * @param query  a string
 * @return a filled query result (in params)
 */
void executeSelect(QueryResult *res, char *query, char *dbPath);

#endif //CBDD1_SELECT_H

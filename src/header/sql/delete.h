//
// Created by Hugo on 15/12/2017.
//

#ifndef CBDD1_DELETE_H
#define CBDD1_DELETE_H

#include "query.h"

/**
 * Execute a delete SQL query
 * @param query
 * @param dbPath path to the db yaml file
 * @return a query result
 */
void executeDelete(QueryResult *res, char *query, char *dbPath);

#endif //CBDD1_DELETE_H

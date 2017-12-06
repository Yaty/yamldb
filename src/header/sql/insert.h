//
// Created by Hugo on 12/11/2017.
//

#ifndef CBDD1_INSERT_H
#define CBDD1_INSERT_H

/**
 * Execute a insert SQL query
 * @param query
 * @return a query result (in params)
 */
void executeInsert(QueryResult *res, char *query, char *dbPath);

#endif //CBDD1_INSERT_H

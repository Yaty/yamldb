//
// Created by Hugo on 11/11/2017.
//

#ifndef CBDD1_QUERY_H
#define CBDD1_QUERY_H

typedef enum QueryStatus { SUCCESS, FAILURE } QueryStatus;

typedef struct QueryResult {
    char ***table;
    char **headers;
    size_t columnsCounter;
    size_t rowsCounter;
    QueryStatus status;
    char *message;
    char **warnings;
    size_t warningsCounter;
} QueryResult;

/**
 * Execute a sql query on a db
 * @param queryString the query in sql
 * @param dbPath path to the db yaml file
 * @return a query result
 */
QueryResult *SQLExecuteQuery(char *queryString, char *dbPath);

void SQLFreeQueryResult(QueryResult *res);

/**
 * Get initialized QueryResult
 * @return a queryresult
 */
QueryResult *getEmptyResult();

/**
 * Get a query result with a failure status and a message
 * @param message
 * @return a query result
 */
QueryResult *getFailedResult(char *message);

/**
 * Print a query result
 */
void SQLPrintQueryResult(QueryResult *res);

#endif //CBDD1_QUERY_H

//
// Created by Hugo on 12/11/2017.
//

#ifndef CBDD1_UTILS_H
#define CBDD1_UTILS_H

#include "structs.h"

/**
 * Get initialized QueryResult
 * @return a queryresult
 */
QueryResult getEmptyResult();

/**
 * Get a query result with a failure status and a message
 * @param message
 * @return a query result
 */
QueryResult getFailedResult(char *message);

/**
 * Get params from a string
 * Example : "param1, param2, param3 bla blabla bla" will return "param1", "param2" and "param3"
 * @param query the string
 * @param paramsCounter the number of params found
 * @return an array of string to free
 */
char **getParams(char *query, int *paramsCounter);

#endif //CBDD1_UTILS_H
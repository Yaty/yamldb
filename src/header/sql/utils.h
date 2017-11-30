//
// Created by Hugo on 12/11/2017.
//

#ifndef CBDD1_UTILS_H
#define CBDD1_UTILS_H

#include "query.h"
#include "../yaml/node.h"

/**
 * Add a warning message to a query result
 * @param result
 * @param warning
 * @return 1 for success, 0 for failure
 */
int addWarningToResult(QueryResult *result, char *warning);

/**
 * Load data and metadata from a table
 * @param dbPath
 * @param currentTable
 * @param data
 * @param metas
 * @return the number of lines
 */
int loadData(char *dbPath, char *currentTable, Node **data, Node **metas);

#endif //CBDD1_UTILS_H

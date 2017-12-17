//
// Created by Hugo on 12/11/2017.
//

#ifndef CBDD1_INSERT_H

#include "query.h"
#include <stdlib.h>

#include <stdio.h>
#include <string.h>
#include "../../header/sql/parser.h"
#include "../../header/string_array_functions.h"
#include "../../header/sql/utils.h"
#include "../../header/yaml/api.h"
#define CBDD1_INSERT_H

/**
 *
 * @param res: query result
 * @param query
 * @param dbPath
 * @param dataPath
 */
void executeInsert(QueryResult *res, char *query, char *dbPath, char *dataPath);

/**
 * allow to collect the column name
 * @param query
 * @param columns: array who contain columns names
 * @param nextVal: limits of the selection
 * @return number of columns
 */
int columnsSizeAndName(char *query, char **columns, size_t *nextVal);

/**
 * allow to know if the number of colums is bigger than number of values
 * @param counterColumns
 * @param counterValuesr
 * @param values
 */
void compareCounters( int counterColumns, int counterValues, char **values );

/**
 *
 * @param values: value who need to change
 * @param childrenNumber: number of children
 * @param i: current position
 * @return
 */
char *modifyStr(char *values, int childrenNumber, int i);

/**
 * Allow to collect table name
 * char *query: request
 * char *space: This is the request before "VALUES" or ('col',...)
 * return table name
 */
size_t tablesSizeAndName(char *query, char *space, char *table);

#endif //CBDD1_INSERT_H

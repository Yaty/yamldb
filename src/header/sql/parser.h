//
// Created by Hugo on 01/12/2017.
//

#ifndef CBDD1_PARSER_H
#define CBDD1_PARSER_H

#include "operators.h"
#include "../../header/sql/conditions.h"
#include "order.h"

/**
 * Get params from a string
 * Example : "param1, param2, param3 bla blabla bla" will return "param1", "param2" and "param3"
 * @param query the string
 * @param paramsCounter the number of params found
 * @return an array of string to free
 */
char **getParams(char *query, int *paramsCounter);

/**
 * Get columns from a query
 * @param query
 * @param columnsCounter
 * @return
 */
char **getColumns(char *query, int *columnsCounter);

/**
 * Get tables from a file
 * @param query
 * @param tablesCounter
 * @return
 */
char **getTables(char *query, int *tablesCounter);

Conditions *getConditions(char *query);

Orders *getOrders(char *query);

/**
 * Get a comparator from a string
 * @param str
 * @param comparator
 * @return the index where the comparator appears
 */
long getComparator(char *str, Comparator *comparator);

/**
 * Get table and column from a string with this format : bla.bla
 * If success, table and column must be freed !
 * @param str
 * @param indexStart
 * @param indexEnd
 * @param table
 * @param column
 * @return 1 for success, 0 for failure
 */
int getTableColumn(char *str, long indexStart, long indexEnd, char **table, char **column);

/**
 * Get the first logical operator from a SQL query strin
 * @param str
 * @param operator
 * @return the index where it occurs
 */
long getLogicalOperator(char *str, LogicalOperator *operator);

/**
 * Get the join type from a SQL query
 * @param str
 * @param index
 * @return
 */
JoinType getJoinType(char *str, long index);

/**
 * Get joins from a SQL query string
 * @param query
 * @return the joins structure
 */
Joins *getJoins(char *query);

#endif //CBDD1_PARSER_H

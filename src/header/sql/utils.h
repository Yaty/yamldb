//
// Created by Hugo on 12/11/2017.
//

#ifndef CBDD1_UTILS_SQL_H
#define CBDD1_UTILS_SQL_H

#include "query.h"
#include "../yaml/node.h"
#include "../utils/hashmap.h"
#include "join.h"

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

/**
 * Add a node by his file path to a hashmap
 * Data key = table
 * Metadata key = table-metadata
 * @param dbPath
 * @param table
 * @param data
 */
void addNodeToHashMap(char *dbPath, char *table, HashMap *data);

/**
 * Free hash map which is filled by node struct
 * @param map
 */
void freeHashMapFilledWithNode(HashMap *map);

/**
 * Return a HashMap filled with data Nodes
 * Key : table name
 * Value : the yml file parsed, browsable in a Node struct
 * @param joins
 * @param dbPath
 * @return the hashmap
 */
HashMap *initDataMap(Joins *joins, char **tables, int tablesCounter, char *dbPath);

/**
 * Get meta node from a hashmap
 * @param dataMap
 * @param table
 * @return
 */
Node *getMetas(HashMap *dataMap, char *table);

/**
 * Remove invalid columns
 * @param columns
 * @param columnsCounter
 * @param tables
 * @param tablesCounter
 * @param res
 * @param dataMap
 */
void removeInvalidColumns(char ***columns, int *columnsCounter, QueryResult *res, HashMap *dataMap);

/**
 * Remove invalid tables
 * @param tables
 * @param tablesCounter
 * @param res
 * @param dataMap
 */
void removeInvalidTables(char ***tables, int *tablesCounter, QueryResult *res, HashMap *dataMap);

/**
 * Check if * is in columns then pick all columns from all table
 * @param columns
 * @param columnsCounter
 * @param tables
 * @param tablesCounter
 * @param dataMap
 */
void handleFullTableSelector(char ***columns, int *columnsCounter, Joins *joins, char **tables, int tablesCounter, HashMap *dataMap);

#endif //CBDD1_UTILS_SQL_H

//
// Created by Hugo on 12/11/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../header/string_array_functions.h"
#include "../../header/sql/utils.h"
#include "../../header/yaml/api.h"

static char **getColumns(char *query, int *columnsCounter) {
    char *queryCpy = toLowerCase(trim(strdup(query)));
    char **res;

    if (queryCpy) {
        long fromIndex = getSubstringIndex(queryCpy, "from") - 1;
        queryCpy[fromIndex] = '\0';
        res = getParams(queryCpy, columnsCounter); // Get columns after select
        free(queryCpy);
        return res;
    }

    return NULL;
}

static char **getTables(char *query, int *tablesCounter) {
    char *queryCpy = toLowerCase(trim(strdup(query)));
    char *tables;
    char **res;

    if (queryCpy) {
        tables = trim(queryCpy + getSubstringIndex(queryCpy, "from") + 5);
        res = getParams(tables, tablesCounter);
        free(queryCpy);
        return res;
    }

    return NULL;
}

// TODO
static char **getConditions(char *query, int *conditionsCounter) {
    char *queryCpy = trim(toLowerCase(strdup(query)));

    if (queryCpy) {
        long whereIndex = getSubstringIndex(queryCpy, "where");

        if (whereIndex > 0) {
            char *queryCpy2 = trim(queryCpy + whereIndex + 5);
            free(queryCpy);
            return NULL; // TODO
        }
    }

    return NULL;
}

static char **getOrders(char *query, int *ordersCounter) {
    return NULL; // TODO
}

/**
 * This function receive a SQL query starting after the SELECT keyword
 * @return fill data into variables passes in parameters, NULL for success, char * for an error msg
 */
static char *parseSelectQuery(char *query, char ***columns, int *columnsCounter, char ***tables, int *tablesCounter, char ***conditions, int *conditionsCounter, char ***orders, int *ordersCounter) {
    if (strlen(query) >= 8) { // c FROM d, the least chars to have a valid query
        int i = 0;

        *columns = getColumns(query, columnsCounter);
        if (*columnsCounter < 1 || *columns == NULL) {
            return "Error 9 : invalid query, please add one or more columns to select.";
        }

        *tables = getTables(query, tablesCounter);
        if (*tablesCounter < 1 || *tables == NULL) {
            for (i = 0; i < *columnsCounter; i++) free(*columns[i]);
            return "Error 8 : invalid query, please add one or more tables.";
        }

        // *conditions = getConditions(query, conditionsCounter); // TODO
        // *orders = getOrders(query, ordersCounter); // TODO

        return NULL; // success
    } else {
        return "Error 11 : invalid query, please respect the given schema in the documentation.";
    }
}

/**
 * Execute a SQL select from a string which is the query
 * @param query  a string
 * @param dbPath path to the db yaml file
 * @return a filled query result
 */
QueryResult executeSelect(char *query, char *dbPath) {
    int i = 0;
    int j = 0;
    int k = 0;
    int dataSize = 0;
    int columnsCounter = 0;
    int tablesCounter = 0;
    int conditionsCounter = 0;
    int ordersCounter = 0;
    int linesCounter = 0;
    int dataAdded = 0;
    int currentLineId = 0;
    int tmpData = 0;
    char ***tmp;
    char **columns;
    char **tables;
    char **conditions;
    char **orders;
    char *parseQuery = parseSelectQuery(query, &columns, &columnsCounter, &tables, &tablesCounter, &conditions, &conditionsCounter, &orders, &ordersCounter);
    char *currentTable;
    char *currentTablePath;
    char *currentTableMetaPath;
    char *currentTableDataPath;
    Node *rootMetas;
    Node *metas;
    Node *rootData;
    Node *data;
    Node *currentLine;
    Node *currentCouple;

    if (parseQuery == NULL && columnsCounter > 0 && tablesCounter > 0) {
        QueryResult res = getEmptyResult();
        res.headers = columns;
        res.columnsCounter = (size_t) columnsCounter;

        for (i = 0; i < tablesCounter; i++) {
            currentTable = tables[i];
            currentTablePath = concat(3, dbPath, "/", currentTable);
            currentTableMetaPath = concat(2, currentTablePath, "/metadata.yml");
            currentTableDataPath = concat(2, currentTablePath, "/data.yml");

            rootMetas = YAMLParseFile(currentTableMetaPath);
            metas = YAMLGetChildByKey(rootMetas, "structure");
            rootData = YAMLParseFile(currentTableDataPath);
            data = YAMLGetChildByKey(rootData, "data");
            dataSize = YAMLGetSize(data);
            linesCounter += dataSize;

            if (rootMetas && rootData && DBIsValidMetadata(metas) && DBIsValidData(data)) { // Valid table
                tmp = realloc(res.table, sizeof(char **) * linesCounter);
                if (tmp) {
                    res.table = tmp;
                    res.rowsCounter = (size_t) linesCounter;

                    for (j = 0; j < dataSize; j++) {
                        res.table[currentLineId] = malloc(sizeof(char*) * columnsCounter);
                        if (res.table[currentLineId]) {
                            currentLine = YAMLGetChildAtIndex(data, j);
                            if (currentLine) {
                                tmpData = dataAdded;

                                for (k = 0; k < columnsCounter; k++) {
                                    currentCouple = YAMLGetChildByKey(currentLine, columns[k]);
                                    if (currentCouple) {
                                        res.table[currentLineId][k] = strdup(YAMLGetValue(currentCouple));
                                        dataAdded += 1;
                                    } else {
                                        addWarningToResult(&res, strdup("Column not found in table line"));
                                    }
                                }

                                if (tmpData + columnsCounter == dataAdded) {
                                    currentLineId += 1;
                                }
                            } else {
                                addWarningToResult(&res, strdup("Line not found."));
                            }
                        } else {
                            addWarningToResult(&res, concat(2, "Internal error : a line will be missing from table : ", currentTable));
                        }
                    }
                } else {
                    addWarningToResult(&res, concat(2, "Internal error, a table has been avoided : ", currentTable));
                }
            } else {
                addWarningToResult(&res, concat(2, "Invalid table : ", currentTable));
            }

            YAMLFreeNode(rootMetas);
            YAMLFreeNode(rootData);
            free(currentTableMetaPath);
            free(currentTableDataPath);
            free(currentTablePath);
        }

        res.status = SUCCESS;

        // Columns are used by the result, so they will be freed later
        for (i = 0; i < tablesCounter; i++) free(tables[i]);
        return res;
    } else {
        return getFailedResult("Error 14 : bad query.");
    }
}
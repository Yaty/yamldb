//
// Created by Hugo on 12/11/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../header/string_array_functions.h"
#include "../../header/sql/utils.h"
#include "../../header/file_manager.h"
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
    int columnsCounter = 0;
    int tablesCounter = 0;
    int conditionsCounter = 0;
    int ordersCounter = 0;
    char **columns;
    char **tables;
    char **conditions;
    char **orders;
    char *parseQuery = parseSelectQuery(query, &columns, &columnsCounter, &tables, &tablesCounter, &conditions, &conditionsCounter, &orders, &ordersCounter);
    char *currentTable;
    char *currentTablePath;
    char *currentTableMetaPath;
    char *currentColumn;

    if (parseQuery == NULL && columnsCounter > 0 && tablesCounter > 0) {
        QueryResult res = getEmptyResult();

        for (i = 0; i < tablesCounter; i++) {
            currentTable = tables[i];
            currentTablePath = concat(3, dbPath, "/", currentTable);
            currentTableMetaPath = concat(2, currentTablePath, "/metadata.yml");

            Node *root = YAMLParseFile(currentTableMetaPath);
            Node *metas = YAMLGetChildByKey(root, "structure");

            if (isFolderExists(currentTablePath) && DBIsValidMetadata(metas)) { // Valid table
                printf("VALID TABLE %s\n", currentTable);
               for (j = 0; j < columnsCounter; j++) {
                   currentColumn = columns[j];
                   Node *currentNodeColumn = YAMLGetChildByKey(metas, currentColumn);
                   if (currentNodeColumn && DBIsValidColumnType(YAMLGetValue(currentNodeColumn))) {
                        printf("VALID COLUMN %s\n", currentColumn);
                       // TODO
                   } else {
                       addWarningToResult(&res, concat(2, "Invalid column : ", currentColumn));
                   }
               }
            } else {
                addWarningToResult(&res, concat(2, "Invalid table : ", currentTable));
            }

            YAMLFreeNode(root);
            free(currentTableMetaPath);
            free(currentTablePath);
        }

        for (i = 0; i < columnsCounter; i++) free(columns[i]);
        for (i = 0; i < tablesCounter; i++) free(tables[i]);
        return res;
    } else {
        return getFailedResult("Error 14 : bad query.");
    }
}
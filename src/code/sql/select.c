//
// Created by Hugo on 12/11/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../header/string_array_functions.h"
#include "../../header/sql/utils.h"
#include "../../header/yaml/api.h"
#include "../../header/sql/join.h"
#include "../../header/sql/parser.h"

/**
 * This function receive a SQL query starting after the SELECT keyword
 * @return fill data into variables passes in parameters, NULL for success, char * for an error msg
 */
static char *parseSelectQuery(char *query, char ***columns, int *columnsCounter, char ***tables, int *tablesCounter, char ***conditions, int *conditionsCounter, char ***orders, int *ordersCounter, Joins *joins, int *jointsCounter) {
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

        *joins = getJoins(query); // TODO
        // *conditions = getConditions(query, conditionsCounter); // TODO
        // *orders = getOrders(query, ordersCounter); // TODO

        return NULL; // success
    } else {
        return "Error 11 : invalid query, please respect the given schema in the documentation.";
    }
}

static int addLine(int columnsCounter, char **columns, Node *currentLine, int currentLineId, HashMap *joinsTable, QueryResult *res) {
    for (int i = 0; i < columnsCounter; i++) {
        Node *currentCouple = YAMLGetChildByKey(currentLine, columns[i]);
        if (currentCouple) {
            res->table[currentLineId][i] = strdup(YAMLGetValue(currentCouple));
        } else if (joinsTable->size > 0) { // Jointure case, we have to found
            int fount = 0;
            for (int j = 0; j < joinsTable->size; j++) {
                Node *currentData = (Node*) joinsTable->values[j];

                for (int k = 0; k < YAMLGetSize(currentData); k++) {
                    Node *currentLine2 = YAMLGetChildAtIndex(currentData, k);
                    currentCouple = YAMLGetChildByKey(currentLine2, columns[i]);
                    if (currentCouple) {
                        res->table[currentLineId][i] = strdup(YAMLGetValue(currentCouple));
                        fount = 1;
                        break;
                    }
                }

                if (fount) {
                    break;
                }
            }
        } else {
            res->table[currentLineId][i] = strdup("NULL");
        }
    }

    return 1;
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
    int l = 0;
    int m = 0;
    int n = 0;
    int dataSize = 0;
    int columnsCounter = 0;
    int tablesCounter = 0;
    int conditionsCounter = 0;
    int ordersCounter = 0;
    int joinsCounter = 0;
    int linesCounter = 0;
    int currentLineId = 0;
    Joins joins;
    char ***tmp;
    char **columns;
    char **tables;
    char **conditions;
    char **orders;
    char *parseQuery = parseSelectQuery(query, &columns, &columnsCounter, &tables, &tablesCounter, &conditions, &conditionsCounter, &orders, &ordersCounter, &joins, &joinsCounter);
    char *currentTable;
    Node *metas;
    Node *data;
    Node *currentLine;
    HashMap *joinsDataMap = NULL;
    JoinField currentField;
    Node *currentColumn1;
    Node *currentJoinTable;
    Node *currentLineJoin;
    Node *currentLineJoinCouple;
    int addThisLine = 1;
    int innerJoinMatch = 0;


    if (parseQuery == NULL && columnsCounter > 0 && tablesCounter > 0) {
        QueryResult res = getEmptyResult();
        res.headers = columns;
        res.columnsCounter = (size_t) columnsCounter;

        joinsDataMap = initDataMap(&joins, dbPath);

        for (i = 0; i < tablesCounter; i++) {
            currentTable = tables[i];
            dataSize = loadData(dbPath, currentTable, &data, &metas);
            linesCounter += dataSize;

            if (DBIsValidMetadata(metas) && DBIsValidData(data)) { // Valid table
                tmp = realloc(res.table, sizeof(char **) * linesCounter);

                if (tmp) {
                    res.table = tmp;
                    res.rowsCounter = (size_t) linesCounter;

                    for (j = 0, addThisLine = 1; j < dataSize; j++) {
                        currentLine = YAMLGetChildAtIndex(data, j);

                        if (currentLine) {
                            for (l = 0; l < joins.joinsNumber; l++) { // For each join
                                switch (joins.joins[i].type) {
                                    case INNER:
                                        for (m = 0; m < joins.joins[i].fieldsNumber; m++) { // For each field in one join
                                            currentField = joins.joins[i].fields[m];
                                            if (areStringsEquals(currentField.originTable, tables[i])) { // If we are in the right table
                                                currentColumn1 = YAMLGetChildByKey(currentLine, currentField.originColumn);
                                                if (currentColumn1) { // The column exists
                                                    currentJoinTable = (Node *) hashLookup(joinsDataMap, currentField.targetTable); // We are sure this key exists
                                                    for (n = 0; n < YAMLGetSize(currentJoinTable); n++) {
                                                        currentLineJoin = YAMLGetChildAtIndex(currentJoinTable, n);
                                                        currentLineJoinCouple = YAMLGetChildByKey(currentLineJoin, currentField.targetColumn);
                                                        innerJoinMatch = areStringsEquals(YAMLGetValue(currentLineJoinCouple), YAMLGetValue(currentColumn1));
                                                        if (innerJoinMatch) {
                                                            addThisLine = evalOperatorInt(addThisLine, innerJoinMatch, currentField.logicOp); // TODO : Handle type and switch evalOperator accordingly
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        break;
                                    case LEFT:
                                        break;
                                    case RIGHT:
                                        break;
                                    case FULL:
                                        break;
                                    default:
                                        break;
                                }
                            }


                            if (addThisLine) {
                                res.table[currentLineId] = malloc(sizeof(char *) * columnsCounter);
                                if (res.table[currentLineId]) {
                                    currentLineId += addLine(columnsCounter, columns, currentLine, currentLineId, joinsDataMap, &res);
                                } else {
                                    addWarningToResult(&res, concat(2, "Internal error : a line will be missing from table : ", currentTable));
                                }
                            }
                        } else {
                            addWarningToResult(&res, strdup("Line not found."));
                        }
                    }
                } else {
                    addWarningToResult(&res, concat(2, "Internal error, a table has been avoided : ", currentTable));
                }
            } else {
                addWarningToResult(&res, concat(2, "Invalid table : ", currentTable));
            }

            YAMLFreeNode(metas);
            YAMLFreeNode(data);
        }

        res.status = res.warningsCounter == 0 ? SUCCESS : FAILURE;

        // Columns are used by the result, so they will be freed later
        for (i = 0; i < tablesCounter; i++) free(tables[i]);
        // for (i = 0; i < joins.joinsNumber; i++) YAMLFreeNode(&joinsTableData[i]);
        // if (joinsTableData) free(joinsTableData);
        return res;
    } else {
        return getFailedResult("Error 14 : bad query.");
    }
}
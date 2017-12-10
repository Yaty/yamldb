//
// Created by Hugo on 12/11/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../header/string_array_functions.h"
#include "../../header/sql/utils.h"
#include "../../header/yaml/api.h"
#include "../../header/sql/parser.h"

static int addLine(Node *line, QueryResult *res, int index, char **columns, int columnsNumber) {
    if (line && res && index >= 0 && YAMLGetSize(line) > 0) {
        int i;
        Node *currentCouple;

        res->table[index] = malloc(sizeof(char *) * YAMLGetSize(line));

        for (i = 0; i < columnsNumber; i++) {
            currentCouple = YAMLGetChildByKey(line, columns[i]);
            if (currentCouple) {
                res->table[index][i] = strdup(YAMLGetValue(currentCouple));
            } else {
                res->table[index][i] = strdup("(NULL)");
            }
        }

        return 1;
    }

    return 0;
}

/**
 * Eval join fields and return columns
 * @param join
 * @param dataMap
 * @param warnings
 * @param warningsNumber
 * @param lineFull
 * @return
 */
static Node *evalJoinFields(Join *join, HashMap *dataMap, char ***warnings, int *warningsNumber, Node *lineFull) {
    if (join && join->fields && join->fieldsNumber > 0 && dataMap) {
        int i;
        int j;
        int res;
        int fieldValues[join->fieldsNumber];
        char *originCell;
        char *originCellType;
        char *targetCellType;
        JoinField field;
        Node *table1;
        Node *table2;
        Node *metas1;
        Node *metas2;
        Node *targetData;

        Node *newLines = YAMLGetMapNode("lines");

        targetData = hashLookup(dataMap, join->target);

        if (!targetData) {
            *warningsNumber += addStringIntoArray(
                    concat(2, "Invalid target table for a join : ", strdup(join->target)),
                    warnings,
                    *warningsNumber
            );
            return NULL;
        }

        for (i = 0; i < YAMLGetSize(targetData); i++) {
            for (j = 0; j < join->fieldsNumber; j++) {
                field = join->fields[j];

                table1 = hashLookup(dataMap, field.originTable);
                metas1 = getMetas(dataMap, field.originTable);
                table2 = hashLookup(dataMap, field.targetTable);
                metas2 = getMetas(dataMap, field.targetTable);

                if (!table1 || !metas1 || !table2 || !metas2) {
                    *warningsNumber += addStringIntoArray(
                            concat(4, "Invalid tables (data or metadata missing for one of them) : ", strdup(field.originTable), " ,", strdup(field.targetTable)),
                            warnings,
                            *warningsNumber
                    );
                    return NULL;
                }

                originCell = YAMLGetValue(YAMLGetChildByKey(lineFull, field.originColumn));
                originCellType = YAMLGetValue(YAMLGetChildByKey(YAMLGetChildByKey(metas1, field.originColumn), "type"));
                targetCellType = YAMLGetValue(YAMLGetChildByKey(YAMLGetChildByKey(metas2, field.targetColumn), "type"));

                if (!originCell) {
                    *warningsNumber += addStringIntoArray(
                            concat(2, "Invalid column data : ", strdup(field.originColumn)),
                            warnings,
                            *warningsNumber
                    );
                    return NULL;
                }

                if (!originCellType || !targetCellType) {
                    *warningsNumber += addStringIntoArray(
                            concat(3, "Invalid column type : ", strdup(field.originColumn), strdup(field.targetColumn)),
                            warnings,
                            *warningsNumber
                    );
                    return NULL;
                }

                // Column type check, we can only compare if they have the same type
                if (!areStringsEquals(originCellType, targetCellType, 1)) {
                    *warningsNumber += addStringIntoArray(
                            concat(
                                    4,
                                    "Invalid join field. You can't compare two columns with a different type : ",
                                    strdup(field.originColumn), " and ", strdup(field.targetColumn)
                            ),
                            warnings,
                            *warningsNumber
                    );
                    return NULL;
                }

                if (YAMLGetChildByKey(lineFull, field.originColumn) == NULL) {
                    *warningsNumber += addStringIntoArray(
                            concat(2, "Invalid column in join field : ", strdup(field.originColumn)),
                            warnings,
                            *warningsNumber
                    );
                    return NULL;
                }


                switch (field.comparator) {
                    case EQUAL:
                        fieldValues[j] = isEqual(
                                originCell,
                                YAMLGetValue(YAMLGetChildByKey(YAMLGetChildAtIndex(targetData, i), field.targetColumn)),
                                originCellType
                        );
                        break;
                    case GREATER:
                        break;
                    case LESSER:
                        break;
                    case GREATER_EQUAL:
                        break;
                    case LESSER_EQUAL:
                        break;
                    case NOT_EQUAL:
                        break;
                    default:
                        *warningsNumber += addStringIntoArray(
                                strdup("Invalid join field comparator : "),
                                warnings,
                                *warningsNumber
                        );
                        return NULL;
                }
            }

            res = 0;
            if (join->fieldsNumber > 1) {
                for (j = 0; j < join->fieldsNumber - 1; j++) {
                    res += evalOperatorInt(fieldValues[j], fieldValues[j + 1], join->fields[j].logicOp);
                }

                if (res == join->fieldsNumber - 1) {
                    YAMLAddChild(newLines, YAMLGetChildAtIndex(targetData, i));
                }
            } else if (fieldValues[0]) {
                YAMLAddChild(newLines, YAMLGetChildAtIndex(targetData, i));
            }
        }

        return newLines;
    }

    return NULL;
}

/**
 * This function retrieve only some columns from a table line
 * @param currentLine
 * @param columns
 * @param columnsCounter
 * @return
 */
static Node *getNewLine(Node *currentLine, char **columns, int columnsCounter) {
    if (currentLine && columns && columnsCounter > 0) {
        int i;
        Node *currentColumn;
        Node *line = YAMLGetMapNode("line");

        for (i = 0; i < YAMLGetSize(currentLine); i++) {
            currentColumn = YAMLGetChildAtIndex(currentLine, i);
            if (stringIntoArray(YAMLGetKey(currentColumn), columns, columnsCounter) || stringIntoArray("*", columns, columnsCounter)) {
                YAMLAddChild(line, currentColumn);
            }
        }

        return line;
    }

    return NULL;
}

/**
 * Make a join on the current line
 * @param currentLine
 * @param joins
 * @param columns
 * @param columnsCounter
 * @param dataMap
 * @param warnings
 * @param warningsNumber
 * @return the join in a node struct
 */
static Node *getNewLineWithJoin(Node *currentLine, Joins *joins, char **columns, int columnsCounter, HashMap *dataMap, char ***warnings, int *warningsNumber) {
    if (currentLine && joins && columns && columnsCounter > 0 && joins->joinsNumber > 0) {
        int i;
        int j;
        int k;

        Node *res = YAMLGetMapNode("lines");
        YAMLAddChild(res, getNewLine(currentLine, columns, columnsCounter)); // first we get value from a normal select
        Node *joinLines;
        Node *joinLine;
        Node *joinCell;
        Node *resLine;
        Node *newLine;

        // Deep copy of currentLine
        Node *wrap = YAMLGetMapNode("line");
        wrap->children = currentLine->children;
        wrap->childrenNumber = currentLine->childrenNumber;
        Node *root = YAMLDeepCopy(wrap);
        Node *lineFull = YAMLGetChildByKey(root, "line");
        YAMLPartialNodeFree(root);

        for (i = 0; i < joins->joinsNumber; i++) { // For each join
            joinLines = evalJoinFields(&joins->joins[i], dataMap, warnings, warningsNumber, lineFull);

            switch (joins->joins[i].type) {
                case INNER:
                    if (YAMLGetSize(joinLines) == 0) {
                        YAMLFreeNode(lineFull);
                        return NULL;
                    }
                    break;
                case FULL:
                    YAMLPrintNode(joinLines);
                    break;
                case LEFT: // by default we have this behaviour so we do not have to do anything
                    break;
                case RIGHT:
                    break;
                default:
                    *warningsNumber += addStringIntoArray(strdup("Invalid join type."), warnings, *warningsNumber);
                    YAMLFreeNode(lineFull);
                    return NULL;
            }

            for (j = 0; j < YAMLGetSize(joinLines); j++) { // For each lines a join generated
                joinLine = YAMLGetChildAtIndex(joinLines, j);
                resLine = YAMLGetChildAtIndex(res, j);
                for (k = 0; k < YAMLGetSize(joinLine); k++) { // For each line of a join
                    joinCell = YAMLGetChildAtIndex(joinLine, k);

                    if (resLine) {
                        YAMLAddChild(resLine, joinCell);
                    } else {
                        // Add a new node and restart the loop to add the line
                        YAMLAddChild(res, getNewLine(currentLine, columns, columnsCounter));
                        j--;
                        break;
                    }
                }
            }
        }

        YAMLFreeNode(lineFull);

        return res;
    }

    return NULL;
}

/**
 * Execute a SQL select from a string which is the query
 * @param query  a string
 * @param dbPath path to the db yaml file
 * @return a filled query result
 */
void executeSelect(QueryResult *res, char *query, char *dbPath) {
    int i = 0;
    int j = 0;
    int k = 0;
    int dataSize = 0;
    int columnsCounter = 0;
    int tablesCounter = 0;
    // int conditionsCounter = 0;
    // int ordersCounter = 0;
    int linesCounter = 0;
    char **columns = getColumns(query, &columnsCounter);
    char **tables = getTables(query, &tablesCounter);
    // char **conditions = getConditions(query, &conditionsCounter); TODO
    // char **orders = getOrders(query, &ordersCounter); TODO
    char *currentTable;
    char ***tmp;
    Joins *joins = getJoins(query);
    Node *metas;
    Node *data;
    Node *currentLine;
    Node *newLines;
    HashMap *dataMap = initDataMap(joins, tables, tablesCounter, dbPath);

    handleFullTableSelector(&columns, &columnsCounter, tables, tablesCounter, dataMap);
    removeInvalidColumns(&columns, &columnsCounter, res, dataMap);
    removeInvalidTables(&tables, &tablesCounter, res, dataMap);

    if (columnsCounter <= 0 || tablesCounter <= 0) {
        addWarningToResult(res, strdup("No valid columns or tables."));
        res->status = FAILURE;
        return;
    }

    res->headers = makeStringsDeepCopy(columns, columnsCounter);
    res->columnsCounter = columnsCounter;
    res->rowsCounter = 0;

    for (i = 0; i < tablesCounter; i++) {
        currentTable = tables[i];

        // Get table data and metadatas from the HashMap
        data = hashLookup(dataMap, currentTable);
        metas = getMetas(dataMap, currentTable);
        dataSize = YAMLGetSize(data);
        linesCounter += dataSize;

        if (!DBIsValidMetadata(metas) || !DBIsValidData(data)) {
            addWarningToResult(res, concat(2, "Invalid table : ", currentTable));
            continue; // go to next table
        }

        tmp = realloc(res->table, sizeof(char **) * linesCounter);

        if (!tmp) {
            addWarningToResult(res, concat(2, "Internal error, a table has been avoided : ", currentTable));
            continue; // go to next table
        }

        res->table = tmp;

        for (j = 0; j < dataSize; j++) {
            currentLine = YAMLGetChildAtIndex(data, j);

            if (joins->joinsNumber > 0) {
                newLines = getNewLineWithJoin(currentLine, joins, columns, columnsCounter, dataMap, &res->warnings, &res->warningsCounter);
                for (k = 0; k < YAMLGetSize(newLines); k++) {
                    res->rowsCounter += addLine(YAMLGetChildAtIndex(newLines, k), res, res->rowsCounter, columns, columnsCounter);
                }
            } else {
                res->rowsCounter += addLine(getNewLine(currentLine, columns, columnsCounter), res, res->rowsCounter, columns, columnsCounter);
            }
        }
    }

    res->status = res->warningsCounter == 0 ? SUCCESS : FAILURE;

    freeHashMapFilledWithNode(dataMap);
    for (i = 0; i < tablesCounter; i++) free(tables[i]);
    for (i = 0; i < columnsCounter; i++) free(columns[i]);
    freeJoins(joins);
}
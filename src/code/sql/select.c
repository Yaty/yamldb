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

static int addLine(Node *line, QueryResult *res, int index) {
    if (line && res && index >= 0) {
        res->table[index] = malloc(sizeof(char *) * YAMLGetSize(line));

        if (res->table[index]) {
            for (int i = 0; i < YAMLGetSize(line); i++) {
                Node *currentCouple = YAMLGetChildAtIndex(line, i);
                if (currentCouple) {
                    res->table[index][i] = strdup(YAMLGetValue(currentCouple));
                } else {
                    res->table[index][i] = strdup("(NULL)");
                }
            }

            return 1;
        }
    }

    return 0;
}

static void freeHashMap(HashMap *map) {
    if (map) {
        for (int i = 0; i < map->size; i++) {
            free(map->keys[i]);
            YAMLFreeNode(map->values[i]);
        }

        free(map);
    }
}

/**
 * Return a HashMap filled with data Nodes
 * Key : table name
 * Value : the yml file parsed, browsable in a Node struct
 * @param joins
 * @param dbPath
 * @return the hashmap
 */
static HashMap *initDataMap(Joins *joins, char **tables, int tablesCounter, char *dbPath) {
    if (dbPath) {
        int i;
        int j;
        int counter = 0;
        Join currentJoin;
        JoinField currentField;

        for (i = 0; i < joins->joinsNumber; i++) {
            currentJoin = joins->joins[i];
            for (j = 0; j < currentJoin.fieldsNumber; j++) {
                currentField = currentJoin.fields[j];
                if (!stringIntoArray(currentField.originTable, tables, tablesCounter)) counter++; // the condition prevent to count twice the same table
                if (!stringIntoArray(currentField.targetTable, tables, tablesCounter)) counter++;
            }
        }

        HashMap *data = hashNew(counter * 2 + tablesCounter * 2); // * 2 to count the metadata file

        for (i = 0; i < joins->joinsNumber; i++) {
            currentJoin = joins->joins[i];
            for (j = 0; j < currentJoin.fieldsNumber; j++) {
                currentField = currentJoin.fields[j];
                if (currentField.originTable) addNodeToHashMap(dbPath, currentField.originTable, data);
                if (currentField.targetTable) addNodeToHashMap(dbPath, currentField.targetTable, data);
            }
        }

        for (i = 0; i < tablesCounter; i++) {
            addNodeToHashMap(dbPath, tables[i], data);
        }

        return data;

    }

    return hashNew(0);
}

static Node *getMetas(HashMap *dataMap, char *table) {
    if (dataMap && table) {
        char *path = concat(2, table, "-metadata");
        Node *metas = hashLookup(dataMap, path);
        free(path);
        return metas;
    }

    return NULL;
}

static Node *evalJoinFields(JoinField *fields, int fieldsNumber, HashMap *dataMap, char ***warnings, size_t *warningsNumber, Node *lineFull, char *target) {
    if (fields && fieldsNumber > 0 && dataMap) {
        int i;
        int j;
        int res;
        int fieldValues[fieldsNumber];
        char *originCell;
        char *originCellType;
        char *targetCellType;
        JoinField field;
        Node *table1;
        Node *table2;
        Node *metas1;
        Node *metas2;
        Node *targetData;

        targetData = hashLookup(dataMap, target);

        if (!targetData) {
            *warningsNumber += addStringIntoArray(
                    concat(2, "Invalid target table for a join : ", target),
                    warnings,
                    *warningsNumber
            );
            return NULL;
        }

        for (i = 0; i < YAMLGetSize(targetData); i++) {
            for (j = 0; j < fieldsNumber; j++) {
                field = fields[j];

                table1 = hashLookup(dataMap, field.originTable);
                metas1 = getMetas(dataMap, field.originTable);
                table2 = hashLookup(dataMap, field.targetTable);
                metas2 = getMetas(dataMap, field.targetTable);

                if (!table1 || !metas1 || !table2 || !metas2) {
                    *warningsNumber += addStringIntoArray(
                            concat(4, "Invalid tables (data or metadata missing for one of them) : ", field.originTable, " ,", field.targetTable),
                            warnings,
                            *warningsNumber
                    );
                    return NULL;
                }

                originCell = YAMLGetValue(YAMLGetChildByKey(lineFull, field.originColumn));
                originCellType = YAMLGetValue(YAMLGetChildByKey(metas1, field.originColumn)); // TODO : HANDLE NEW METADATA STRUCTURE
                targetCellType = YAMLGetValue(YAMLGetChildByKey(metas2, field.targetColumn)); // TODO : IDEM

                if (!originCell) {
                    *warningsNumber += addStringIntoArray(
                            concat(2, "Invalid column data : ", field.originColumn),
                            warnings,
                            *warningsNumber
                    );
                    return NULL;
                }

                if (!originCellType || !targetCellType) {
                    *warningsNumber += addStringIntoArray(
                            concat(3, "Invalid column type : ", field.originColumn, field.targetColumn),
                            warnings,
                            *warningsNumber
                    );
                    return NULL;
                }

                // Column type check, we can only compare if they have the same type TODO HANDLE NEW METADATA STRUCTURE
                if (!areStringsEquals(originCellType, targetCellType)) {
                    *warningsNumber += addStringIntoArray(
                            concat(
                                    4,
                                    "Invalid join field. You can't compare two columns with a different type : ",
                                    field.originColumn, " and ", field.targetColumn
                            ),
                            warnings,
                            *warningsNumber
                    );
                    return NULL;
                }

                if (YAMLGetChildByKey(lineFull, field.originColumn) == NULL) {
                    *warningsNumber += addStringIntoArray(
                            concat(2, "Invalid column in join field : ", field.originColumn),
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
            if (fieldsNumber > 1) {
                for (j = 0; j < fieldsNumber - 1; j++) {
                    res += evalOperatorInt(fieldValues[j], fieldValues[j + 1], fields[j].logicOp);
                }

                if (res == fieldsNumber - 1) {
                    return YAMLGetChildAtIndex(targetData, i);
                }
            } else if (fieldValues[0]) {
                return YAMLGetChildAtIndex(targetData, i);
            }
        }
    }

    return NULL;
}

static Node *makeJoin(Join *join, char **columns, int columnsCounter, HashMap *dataMap, char ***warnings, size_t *warningsNumber, Node *lineFull) {
    if (join && columns && columnsCounter > 0 && dataMap && warnings && lineFull) {
        Node *joinRes = evalJoinFields(join->fields, join->fieldsNumber, dataMap, warnings, warningsNumber, lineFull, join->target);
        if (joinRes) {
            switch (join->type) {
                case INNER:
                    return joinRes;
                case FULL:
                    break;
                case LEFT:
                    break;
                case RIGHT:
                    break;
                default:
                    break;
            }
        }
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
            if (stringIntoArray(YAMLGetKey(currentColumn), columns, columnsCounter)) {
                YAMLAddChild(line, currentColumn);
            }
        }

        return line;
    }

    return NULL;
}

static Node *getNewLineWithJoin(Node *currentLine, Joins *joins, char **columns, int columnsCounter, HashMap *dataMap, char ***warnings, size_t *warningsNumber) {
    if (currentLine && joins && columns && columnsCounter > 0 && joins->joinsNumber > 0) {
        int i;
        int j;

        Node *res = getNewLine(currentLine, columns, columnsCounter); // first we get value from a normal select
        Node *joinLine;
        Node *joinCell;

        Node *wrap = YAMLGetMapNode("line");
        wrap->children = currentLine->children;
        wrap->childrenNumber = currentLine->childrenNumber;
        Node *root = YAMLDeepCopy(wrap);
        Node *lineFull = YAMLGetChildByKey(root, "line");
        YAMLPartialNodeFree(root);

        for (i = 0; i < joins->joinsNumber; i++) { // For each join
            joinLine = makeJoin(&joins->joins[i], columns, columnsCounter, dataMap, warnings, warningsNumber, lineFull);
            for (j = 0; j < YAMLGetSize(joinLine); j++) { // For each column from a the join
                joinCell = YAMLGetChildAtIndex(joinLine, j);
                if (YAMLGetChildByKey(res, YAMLGetKey(joinCell)) == NULL) { // Check if the column is not here
                    YAMLAddChild(res, joinCell); // Add column: value into our result
                    YAMLAddChild(lineFull, joinCell);
                } else {
                    *warningsNumber += addStringIntoArray(strdup("Duplicated column during a join."), warnings, *warningsNumber);
                }
            }
        }

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
    int dataSize = 0;
    int columnsCounter = 0;
    int tablesCounter = 0;
    // int conditionsCounter = 0;
    // int ordersCounter = 0;
    int linesCounter = 0;
    int currentLineId = 0;
    Joins *joins = getJoins(query);
    char **columns = getColumns(query, &columnsCounter);
    char **tables = getTables(query, &tablesCounter);
    // char **conditions = getConditions(query, &conditionsCounter); TODO
    // char **orders = getOrders(query, &ordersCounter); TODO
    char *currentTable;
    char ***tmp;
    char *metasName = NULL;
    Node *metas;
    Node *data;
    Node *currentLine;
    HashMap *dataMap = NULL;
    Node *newLine;

    if (columnsCounter <= 0 || tablesCounter <= 0) {
        SQLFreeQueryResult(res);
        res = getFailedResult("Bad query.");
        return;
    }

    res->headers = columns;
    res->columnsCounter = (size_t) columnsCounter;
    dataMap = initDataMap(joins, tables, tablesCounter, dbPath);

    YAMLPrintNode(hashLookup(dataMap, "table2")); // TODO FIX PARSER ONLY GET FIRST NUMBER
    // TODO FIX COLUMN ORDERING IN JOIN

    for (i = 0; i < tablesCounter; i++) {
        currentTable = tables[i];

        // Get table data and metadatas from the HashMap
        data = hashLookup(dataMap, currentTable);
        metasName = concat(2, currentTable, "-metadata");
        metas = hashLookup(dataMap, metasName);
        free(metasName);
        dataSize = loadData(dbPath, currentTable, &data, &metas);
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
        res->rowsCounter = (size_t) linesCounter;

        for (j = 0; j < dataSize; j++) {
            currentLine = YAMLGetChildAtIndex(data, j);

            if (!currentLine) {
                addWarningToResult(res, strdup("Line not found."));
                continue; // go to next line in table
            }

            if (joins->joinsNumber > 0) {
                newLine = getNewLineWithJoin(currentLine, joins, columns, columnsCounter, dataMap, &res->warnings, &res->warningsCounter);
            } else {
                newLine = getNewLine(currentLine, columns, columnsCounter);
            }

            if (newLine) {
                currentLineId += addLine(newLine, res, currentLineId);
            }
        }

        YAMLFreeNode(metas);
        YAMLFreeNode(data);
    }

    res->status = res->warningsCounter == 0 ? SUCCESS : FAILURE;

    // Columns are used by the result, so they will be freed later
    for (i = 0; i < tablesCounter; i++) free(tables[i]);
    if (dataMap) freeHashMap(dataMap);
    // for (i = 0; i < joins.joinsNumber; i++) YAMLFreeNode(&joinsTableData[i]);
    // if (joinsTableData) free(joinsTableData);
}
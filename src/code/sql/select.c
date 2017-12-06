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

    /*
      else if (joinsTable->size > 0) { // Jointure case, we have to found
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
     */
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

static void execJoinRequest(QueryResult *res, char **tables, int tablesCounter, char **columns, int columnsCounter, HashMap *dataMap, Joins *joins, char *dbPath) {
    int i;
    int j;
    int k;
    int l;
    int m;
    int dataSize;
    int linesCounter = 0;
    int addThisLine;
    int innerJoinMatch;
    int currentLineId = 0;

    char *currentTable;
    char *metasName;
    char ***tmp;

    Node *data;
    Node *metas;
    Node *currentLine;
    Node *currentJoinFieldTable1;
    Node *currentJoinFieldTable2;
    Node *currentColumn1;
    Node *currentJoinTable;
    Node *currentLineJoin;
    Node *currentLineJoinCouple;

    JoinField currentField;


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
        for (j = 0, addThisLine = 1; j < dataSize; j++) {
            currentLine = YAMLGetChildAtIndex(data, j);

            if (!currentLine) {
                addWarningToResult(res, strdup("Line not found."));
                continue; // go to next line in table
            }

            for (k = 0; k < joins->joinsNumber; k++) { // For each join
                switch (joins->joins[i].type) {
                    case INNER:
                        for (l = 0; l < joins->joins[i].fieldsNumber; l++) { // For each field in one join
                            currentField = joins->joins[i].fields[l];
                            currentJoinFieldTable1 = hashLookup(dataMap, currentField.originTable);
                            currentJoinFieldTable2 = hashLookup(dataMap, currentField.targetTable);

                            if (!currentJoinFieldTable1 || !currentJoinFieldTable2) continue;

                            currentColumn1 = YAMLGetChildByKey(currentLine, currentField.originColumn);
                            if (!currentColumn1) continue;

                            currentJoinTable = hashLookup(dataMap, currentField.targetTable); // We are sure this key exists
                            for (m = 0; m < YAMLGetSize(currentJoinTable); m++) {
                                currentLineJoin = YAMLGetChildAtIndex(currentJoinTable, m);
                                currentLineJoinCouple = YAMLGetChildByKey(currentLineJoin, currentField.targetColumn);
                                innerJoinMatch = areStringsEquals(YAMLGetValue(currentLineJoinCouple), YAMLGetValue(currentColumn1)); // TODO : Handle comparaison according to the data type
                                if (innerJoinMatch) {
                                    if (joins->joins[i].fieldsNumber > 1) { // No need the eval if there is only one field
                                        addThisLine = evalOperatorInt(addThisLine, innerJoinMatch, currentField.logicOp); // TODO : Handle type and switch evalOperator accordingly
                                    }
                                } else if (joins->joins[i].fieldsNumber == 1) {
                                    addThisLine = 0;
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
                res->table[currentLineId] = malloc(sizeof(char *) * columnsCounter);
                if (res->table[currentLineId]) {
                    // currentLineId += addLine(columnsCounter, columns, currentLine, currentLineId, dataMap, res);
                } else {
                    addWarningToResult(res, concat(2, "Internal error : a line will be missing from table : ", currentTable));
                }
            }
        }

        YAMLFreeNode(metas);
        YAMLFreeNode(data);
    }
}

static int evalJoinField(JoinField joinField, HashMap *dataMap) {
    int i;
    int j;
    Node *originLine;
    Node *originCell;
    Node *targetLine;
    Node *targetCell;
    Node *table1 = hashLookup(dataMap, joinField.originTable);
    Node *metas1 = getMetas(dataMap, joinField.originTable);
    Node *table2 = hashLookup(dataMap, joinField.targetTable);
    Node *metas2 = getMetas(dataMap, joinField.targetTable);

    if (!table1 || !metas1 || !table2 || !metas2) {
        return 0;
    }

    // Column type check
    if (!areStringsEquals(
            YAMLGetValue(YAMLGetChildByKey(metas1, joinField.originColumn)),
            YAMLGetValue(YAMLGetChildByKey(metas2, joinField.targetTable)))) {
        return 0;
    }

    for (i = 0; i < YAMLGetSize(table1); i++) {
        originLine = YAMLGetChildAtIndex(table1, i);
        originCell = YAMLGetChildByKey(originLine, joinField.originColumn);

        for (j = 0; j < YAMLGetSize(table2); j++) { // For each line of the target table
            targetLine = YAMLGetChildAtIndex(table2, j);
            targetCell = YAMLGetChildByKey(targetLine, joinField.targetColumn);

            if (areStringsEquals(joinField.originColumn, "integer")) {
                if (evalComparatorInt(
                        (int) strtol(YAMLGetValue(originCell), NULL, 10), // STR to int
                        (int) strtol(YAMLGetValue(targetCell), NULL, 10),
                        joinField.comparator
                )) {

                }
            }
        }
    }
}

static int evalJoin(Join join, HashMap *dataMap) {
    int i;
    int res = 0;

    for (i = 0; i < join.fieldsNumber; i++) {
        res += evalJoinField(join.fields[i], dataMap);
    }

    return res > 0;
}

static Node *makeJoin(Join join, char **columns, int columnsCounter, HashMap *dataMap, char ***warnings, size_t *warningsNumber) {
    // TODO :
    /*
     * HANDLE JOIN TYPES
     * GET THE FIRST MATCH FROM ALL JOIN FIELDS THEN RETURN IT IN A NODE
     * COLUMNS THAT WE WANT ARE IN **columns
     * line:
     *  col1: val1
     */
}

static Node *getNewLineWithJoin(Node *currentLine, Joins *joins, char **columns, int columnsCounter, HashMap *dataMap, char ***warnings, size_t *warningsNumber) {
    if (currentLine && joins && columns && columnsCounter > 0) {
        int i;
        int j;

        Node *res = YAMLGetMapNode("line");
        Node *joinLine;
        Node *joinCell;

        for (i = 0; i < joins->joinsNumber; i++) { // For each join
            joinLine = makeJoin(joins->joins[i], columns, columnsCounter, dataMap, warnings, warningsNumber);
            for (j = 0; j < YAMLGetSize(joinLine); j++) { // For each column from a the join
                joinCell = YAMLGetChildAtIndex(joinLine, j);
                if (YAMLGetChildByKey(res, YAMLGetKey(joinCell)) == NULL) { // Check if the column is not here
                    YAMLAddChild(res, joinCell); // Add column: value into our result
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
    int l = 0;
    int m = 0;
    int n = 0;
    int o = 0;
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
    JoinField currentField;
    Node *currentColumn1;
    Node *currentJoinTable;
    Node *currentLineJoin;
    Node *currentLineJoinCouple;
    Node *currentJoinFieldTable1;
    Node *currentJoinFieldTable2;
    Node *newLine;
    int addThisLine = 1;
    int innerJoinMatch = 0;

    if (columnsCounter <= 0 || tablesCounter <= 0) {
        SQLFreeQueryResult(res);
        res = getFailedResult("Bad query.");
        return;
    }

    res->headers = columns;
    res->columnsCounter = (size_t) columnsCounter;
    dataMap = initDataMap(joins, tables, tablesCounter, dbPath);

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
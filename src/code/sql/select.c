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
 *
 * @param str
 * @param index
 * @return
 */
static JoinType getJoinType(char *str, long index) {
    if (str) { // a from t (8)JOIN_TYPE join
        char *strCpy = strdup(str);
        char *p;
        JoinType type = NONE;
        if (strCpy) {
            strCpy[index] = '\0';
            trim(strCpy);
            p = strrchr(strCpy, ' ');
            if (p && *(p + 1)) {
                p += 1;
                trim(p);

                if (areStringsEquals(p, "left")) {
                    type = LEFT;
                } else if (areStringsEquals(p, "right")) {
                    type =  RIGHT;
                } else if (areStringsEquals(p, "full")) {
                    type =  FULL;
                } else {
                    type =  INNER;
                }
            }

            free(strCpy);
            return type;
        }
    }

    return NONE;
}

static long getComparator(char *str, Comparator *comparator) {
    if (str && comparator) {
        long compIndexes[] = {
                getSubstringIndex(str, "="),
                getSubstringIndex(str, ">"),
                getSubstringIndex(str, "<"),
                getSubstringIndex(str, ">="),
                getSubstringIndex(str, "<="),
                getSubstringIndex(str, "<>")
        };

        short size = sizeof(compIndexes) / sizeof(compIndexes[0]);
        short i = 0;
        short minIndex = -1;
        long minComparatorIndex = -1;
        short defined = 0;

        for (i = 0; i < size; i++) {
            if (compIndexes[i] != -1) {
                if (defined == 0) {
                    minIndex = i;
                    minComparatorIndex = compIndexes[i];
                    defined = 1;
                } else {
                    if (compIndexes[i] < minComparatorIndex) {
                        minIndex = i;
                    }
                }
            }
        }

        if (minComparatorIndex > 0 && minIndex >= 0) {
            *comparator = (Comparator) minIndex + 1; // + 1 to ignore NO_COMPARATOR
            return minComparatorIndex;
        }
    }

    if (comparator) {
        *comparator = NO_COMPARATOR;
    }
    return -1;
}

/**
 * Get table and column from a string with this format : bla.bla
 * If success, table and column must be freed !
 * @param str
 * @param indexStart
 * @param indexEnd
 * @param table
 * @param column
 * @return
 */
static int getTableColumn(char *str, long indexStart, long indexEnd, char **table, char **column) {
    if (str && table && column && indexStart >= 0 && indexEnd > indexStart + 1) {
        long pointIndex = getSubstringIndex(str, ".");
        if (pointIndex > 0) {
            *table = malloc(sizeof(char) * pointIndex);
            *column = malloc(sizeof(char) * (indexEnd - indexStart - pointIndex));
            if (*table && *column) {
                int success1 = substring(str, *table, 0, (size_t) pointIndex);
                int success2 = substring(str, *column, (int) pointIndex + 1, (size_t) indexEnd - pointIndex);
                if (success1 && success2) {
                    *table = trim(*table);
                    *column = trim(*column);
                    return 1;
                }
            }
        }
    }

    return 0;
}

static long getLogicalOperator(char *str, LogicalOperator *operator) {
    if (str && operator) {
        long opeIndexes[] = {
                getSubstringIndex(str, "and"),
                getSubstringIndex(str, "or"),
                getSubstringIndex(str, "any"),
                getSubstringIndex(str, "between"),
                getSubstringIndex(str, "exists"),
                getSubstringIndex(str, "in"),
                getSubstringIndex(str, "like"),
                getSubstringIndex(str, "not"),
                getSubstringIndex(str, "some"),
        };

        short i = 0;
        short size = sizeof(opeIndexes) / sizeof(opeIndexes[0]);
        short minIndex = -1;
        long minOperatorIndex = -1;
        short defined = 0;

        for (i = 0; i < size; i++) {
            if (opeIndexes[i] != -1) {
                if (defined == 0) {
                    minIndex = i;
                    minOperatorIndex = opeIndexes[i];
                    defined = 1;
                } else {
                    if (opeIndexes[i] < minOperatorIndex) {
                        minIndex = i;
                    }
                }
            }
        }

        if (minOperatorIndex > 0 && minIndex >= 0) {
            *operator = (LogicalOperator) minIndex + 1; // + 1 to ignore NO_OPERATOR
            return minOperatorIndex;
        }
    }

    if (*operator) {
        *operator = NO_OPERATOR;
    }
    return -1;
}

static Joins getJoins(char *query) {
    Joins joins = getEmptyJoins();

    if (query) {
        char *queryCpy = toLowerCase(trim(strdup(query)));
        char *ptr = queryCpy;
        char *target = NULL;
        char *table1;
        char *col1;
        char *table2;
        char *col2;
        long index;
        long indexSpace;
        long indexOn;
        long indexOperator;
        Join *joinsTmp = NULL;
        Join newJoin;
        JoinType type;
        JoinField *fields = NULL;
        JoinField *fieldsTmp = NULL;
        Comparator currentComparator;
        int fieldsNumber = 0;

        while ((index = getSubstringIndex(ptr, "join")) != -1) {
            type = getJoinType(ptr, index);
            ptr += index + 4; // make ptr point the the space after it
            ptr = trim(ptr);

            if (type) {
                indexSpace = getSubstringIndex(ptr, " "); // ptr ~= "t on bla.bla = bla.bla ..."
                if (indexSpace > 0) {
                    target = strdup(ptr);
                    target[indexSpace] = '\0';
                    ptr += indexSpace; // ptr ~= "on bla.bla = bla.bla ..."
                    ptr = trim(ptr);
                    indexOn = getSubstringIndex(ptr, "on");
                    if (indexOn == 0) {
                        ptr += 2; // ptr ~= "bla.bla = bla.bla ..."
                        ptr = trim(ptr);

                        do {
                            indexOperator = getComparator(ptr, &currentComparator);
                            if (indexOperator > 2 && getTableColumn(ptr, 0, indexOperator - 1, &table1, &col1)) { // b.b=...
                                ptr += indexOperator + 1;
                                ptr = trim(ptr);
                                if (getTableColumn(ptr, 0, indexOperator - 1, &table2, &col2)) {
                                    fieldsTmp = realloc(fields, (size_t) fieldsNumber + 1);
                                    if (fieldsTmp) {
                                        fields = fieldsTmp;
                                        fields[fieldsNumber] = getEmptyJoinField();
                                        fields[fieldsNumber].originTable = table1;
                                        fields[fieldsNumber].originColumn = col1;
                                        fields[fieldsNumber].targetTable = table2;
                                        fields[fieldsNumber].targetColumn = col2;
                                        fields[fieldsNumber].comparator = currentComparator;
                                        fields[fieldsNumber].logicOp = NO_OPERATOR; // We will detect this after
                                        fieldsNumber++;
                                    }
                                }
                            }
                        } while ((indexOperator = getLogicalOperator(ptr, &fields[fieldsNumber].logicOp)) >= 0 && indexOperator < getSubstringIndex(ptr, "join"));

                        joinsTmp = realloc(joins.joins, (size_t) joins.joinsNumber + 1);
                        if (joinsTmp) {
                            newJoin = getEmptyJoin();
                            newJoin.type = type;
                            newJoin.fields = fields;

                            joins.joins = joinsTmp;
                            joins.joins[joins.joinsNumber++] = newJoin;
                        }
                    }
                }
            }
        }

        free(queryCpy);
    }

    return joins;
}

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
                                for (k = 0; k < columnsCounter; k++) {
                                    // TODO : Support joint request here
                                    // We should open each DB from the join request
                                    // then do our stuff
                                    currentCouple = YAMLGetChildByKey(currentLine, columns[k]);
                                    res.table[currentLineId][k] = currentCouple ? strdup(YAMLGetValue(currentCouple)) : strdup("NULL");
                                }

                                currentLineId += 1;
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

        res.status = res.warningsCounter == 0 ? SUCCESS : FAILURE;

        // Columns are used by the result, so they will be freed later
        for (i = 0; i < tablesCounter; i++) {
            free(tables[i]);
        }
        return res;
    } else {
        return getFailedResult("Error 14 : bad query.");
    }
}
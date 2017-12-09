//
// Created by Hugo on 01/12/2017.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../header/string_array_functions.h"
#include "../../header/sql/operators.h"
#include "../../header/sql/join.h"

/**
 * Get params from a string
 * Example : "param1, param2, param3 bla blabla bla" will return "param1", "param2" and "param3"
 * @param query the string
 * @param paramsCounter the number of params found
 * @return an array of string to free
 */
char **getParams(char *query, int *paramsCounter) {
    if (query && paramsCounter) {
        char *queryCpy = strdup(query);
        char *trimmedParam;
        char *pt = strtok(queryCpy, ",");
        char **params = NULL;
        char **temp = NULL;
        int i = 0;
        long index = 0;
        *paramsCounter = 0;

        if (pt == NULL) {
            free(queryCpy);
            return NULL;
        }

        while (pt != NULL) {
            temp = realloc(params, sizeof(char*) * (*paramsCounter + 1));
            if (temp) {
                params = temp;
                trimmedParam = trim(strdup(pt)); // will be freed later
                index = getSubstringIndex(trimmedParam, " ", 0);
                if (index > 0) { // Keep the first word
                    trimmedParam[index] = '\0';
                }
                params[(*paramsCounter)++] = trimmedParam;
                pt = strtok (NULL, ",");
            } else {
                free(queryCpy);
                for (i = 0; i < *paramsCounter; i++) free(params[i]);
                return NULL;
            }
        }

        return params;
    }

    return NULL;
}

/**
 * Get columns from a query
 * @param query
 * @param columnsCounter
 * @return
 */
char **getColumns(char *query, int *columnsCounter) {
    char *queryCpy = trim(strdup(query));
    char **res;

    if (queryCpy) {
        long fromIndex = getSubstringIndex(queryCpy, "from", 1) - 1;
        queryCpy[fromIndex] = '\0';
        res = getParams(queryCpy, columnsCounter); // Get columns after select
        free(queryCpy);
        return res;
    }

    return NULL;
}

/**
 * Get tables from a file
 * @param query
 * @param tablesCounter
 * @return
 */
char **getTables(char *query, int *tablesCounter) {
    char *queryCpy = trim(strdup(query));
    char *tables;
    char **res;

    if (queryCpy) {
        tables = trim(queryCpy + getSubstringIndex(queryCpy, "from", 1) + 5);
        res = getParams(tables, tablesCounter);
        free(queryCpy);
        return res;
    }

    return NULL;
}

// TODO
char **getConditions(char *query, int *conditionsCounter) {
    char *queryCpy = trim(strdup(query));

    if (queryCpy) {
        long whereIndex = getSubstringIndex(queryCpy, "where", 1);

        if (whereIndex > 0) {
            char *queryCpy2 = trim(queryCpy + whereIndex + 5);
            free(queryCpy);
            return NULL; // TODO
        }
    }

    return NULL;
}

// TODO
char **getOrders(char *query, int *ordersCounter) {
    return NULL; // TODO
}

/**
 * Get a comparator from a string
 * @param str
 * @param comparator
 * @return the index where the comparator appears
 */
long getComparator(char *str, Comparator *comparator) {
    if (str && comparator) {
        long compIndexes[] = {
                getSubstringIndex(str, "=", 0),
                getSubstringIndex(str, ">", 0),
                getSubstringIndex(str, "<", 0),
                getSubstringIndex(str, ">=", 0),
                getSubstringIndex(str, "<=", 0),
                getSubstringIndex(str, "<>", 0)
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
 * @return 1 for success, 0 for failure
 */
int getTableColumn(char *str, long indexStart, long indexEnd, char **table, char **column) {
    if (str && table && column && indexStart >= 0 && indexEnd > indexStart + 1) {
        long pointIndex = getSubstringIndex(str, ".", 0);
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
                } else {
                    free(*table);
                    free(*column);
                    *table = NULL;
                    *column = NULL;
                }
            }
        }
    }

    return 0;
}

/**
 * Get the first logical operator from a SQL query strin
 * @param str
 * @param operator
 * @return the index where it occurs
 */
long getLogicalOperator(char *str, LogicalOperator *operator) {
    if (str && operator) {
        long opeIndexes[] = {
                getSubstringIndex(str, "and", 1),
                getSubstringIndex(str, "or", 1),
                getSubstringIndex(str, "any", 1),
                getSubstringIndex(str, "between", 1),
                getSubstringIndex(str, "exists", 1),
                getSubstringIndex(str, "in", 1),
                getSubstringIndex(str, "like", 1),
                getSubstringIndex(str, "not", 1),
                getSubstringIndex(str, "some", 1),
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

    if (operator) {
        *operator = NO_OPERATOR;
    }
    return -1;
}

/**
 * Get the join type from a SQL query
 * @param str
 * @param index
 * @return
 */
JoinType getJoinType(char *str, long index) {
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

                if (areStringsEquals(p, "left", 1)) {
                    type = LEFT;
                } else if (areStringsEquals(p, "right", 1)) {
                    type = RIGHT;
                } else if (areStringsEquals(p, "full", 1)) {
                    type = FULL;
                } else {
                    type = INNER;
                }
            }

            free(strCpy);
            return type;
        }
    }

    return NONE;
}

/**
 * Get joins from a SQL query string
 * @param query
 * @return the joins structure
 */
Joins* getJoins(char *query) {
    Joins *joins = getEmptyJoins();

    if (query) {
        char *queryCpy = trim(strdup(query));
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
        Join *newJoin;
        JoinType type;
        JoinField *fields = NULL;
        JoinField *fieldsTmp = NULL;
        Comparator currentComparator;
        int fieldsNumber = 0;

        while ((index = getSubstringIndex(ptr, "join", 1)) != -1) {
            type = getJoinType(ptr, index);
            ptr += index + 4; // make ptr point the the space after it
            ptr = trim(ptr);

            if (type) {
                indexSpace = getSubstringIndex(ptr, " ", 0); // ptr ~= "t on bla.bla = bla.bla ..."
                if (indexSpace > 0) {
                    target = strdup(ptr);
                    target[indexSpace] = '\0';
                    ptr += indexSpace; // ptr ~= "on bla.bla = bla.bla ..."
                    ptr = trim(ptr);
                    indexOn = getSubstringIndex(ptr, "on", 1);
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
                                        fields[fieldsNumber] = *getEmptyJoinField();
                                        fields[fieldsNumber].originTable = table1;
                                        fields[fieldsNumber].originColumn = col1;
                                        fields[fieldsNumber].targetTable = table2;
                                        fields[fieldsNumber].targetColumn = col1;
                                        fields[fieldsNumber].comparator = currentComparator;
                                        indexOperator = getLogicalOperator(ptr, &fields[fieldsNumber].logicOp); // This will set logicOp
                                        fieldsNumber++;
                                    }
                                }
                            }
                        } while (indexOperator >= 0 && indexOperator < getSubstringIndex(ptr, "join", 1));

                        joinsTmp = realloc(joins->joins, (size_t) joins->joinsNumber + 1);
                        if (joinsTmp) {
                            newJoin = getEmptyJoin();
                            newJoin->type = type;
                            newJoin->fields = fields;
                            newJoin->target = strdup(target);
                            newJoin->fieldsNumber = fieldsNumber;

                            joins->joins = joinsTmp;
                            joins->joins[joins->joinsNumber++] = *newJoin;
                        }
                        free(target);
                    }
                }
            }
        }

        free(queryCpy);
    }

    return joins;
}
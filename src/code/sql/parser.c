//
// Created by Hugo on 01/12/2017.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../header/string_array_functions.h"
#include "../../header/sql/operators.h"
#include "../../header/sql/join.h"
#include "../../header/sql/parser.h"
#include "../../header/sql/order.h"

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
    char *queryCpy = strdup(query);
    char *savePtrPos = queryCpy;
    char **res;
    long fromIndex;

    if (queryCpy) {
        queryCpy = trim(queryCpy);
        fromIndex = getSubstringIndex(queryCpy, "from", 1) - 1;
        queryCpy[fromIndex] = '\0';
        res = getParams(queryCpy, columnsCounter); // Get columns after select
        free(savePtrPos);
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
    char *queryCpy = strdup(query);
    char *savePtrPos = queryCpy;
    char *tables;
    char **res;
    long orderByIndex;

    if (queryCpy) {
        queryCpy = trim (queryCpy);
        tables = trim(queryCpy + getSubstringIndex(queryCpy, "from", 1) + 5);
        res = getParams(tables, tablesCounter);

        if ((orderByIndex = getSubstringIndex(queryCpy, "order by", 1)) > 0) {
            if (getSubstringIndex(queryCpy, ",", 0) > orderByIndex) {
                *tablesCounter -= removeElementAtIndex(&res, *tablesCounter, (*tablesCounter) - 1, 1);
            }
        }


        free(savePtrPos);
        return res;
    }

    return NULL;
}

Conditions *getConditions(char *query) {
    if (query) {
        char *queryCpy = strdup(query);
        char *savePtrPosition = queryCpy;
        char *key;
        char *value;
        Conditions *c = NULL;
        Condition *condition = NULL;
        Condition *tmp = NULL;
        Comparator currentComparator;
        LogicalOperator currentLogicalOperator;
        LogicalOperator *tmp2 = NULL;
        long comparatorIndex = -1;
        long logicalOperatorIndex = -1;
        long spaceIndex = -1;
        long whereIndex = -1;
        int success1 = 0;
        int success2 = 0;
        long orderByIndex = -1;
        size_t valueLength;

        if (queryCpy) {
            queryCpy = trim(queryCpy);
            whereIndex = getSubstringIndex(queryCpy, "where", 1);

            if (whereIndex > 0) {
                c = getEmptyConditions();
                queryCpy = trim(queryCpy + whereIndex + 5); // 5 -> WHERE length

                do {
                    if (logicalOperatorIndex > 0) {
                        orderByIndex = getSubstringIndex(queryCpy, "order by", 1);

                        if (logicalOperatorIndex == orderByIndex) { // it will detect the OR of ORDER BY as an operator
                            break;
                        }

                        queryCpy += logicalOperatorIndex;
                        queryCpy = trim(queryCpy);

                        spaceIndex = getSubstringIndex(queryCpy, " ", 0);

                        if (spaceIndex > 0) {
                            queryCpy += spaceIndex;
                            queryCpy = trim(queryCpy);
                            tmp2 = realloc(c->operators, sizeof(LogicalOperator) * (c->operatorsNumber + 1));
                            if (tmp2) {
                                c->operators = tmp2;
                                c->operators[c->operatorsNumber++] = currentLogicalOperator;
                            }
                        } else {
                            break;
                        }
                    }

                    success1 = 0;
                    success2 = 0;
                    key = NULL;
                    value = NULL;

                    comparatorIndex = getComparator(queryCpy, &currentComparator);
                    if (comparatorIndex > 2) { // c = ...
                        key = malloc(((size_t) comparatorIndex + 1) * sizeof(char));
                        if (key) {
                            success1 = substring(queryCpy, key, 0, (size_t) comparatorIndex);
                            queryCpy += comparatorIndex;
                            queryCpy = trim(queryCpy);
                            spaceIndex = getSubstringIndex(queryCpy, " ", 0);
                            if (spaceIndex > 0) {
                                queryCpy += spaceIndex;
                                queryCpy = trim(queryCpy);
                                spaceIndex = getSubstringIndex(queryCpy, " ", 0);
                                if (spaceIndex > 0) {
                                    value = malloc(((size_t) spaceIndex + 1) * sizeof(char));
                                    if (value) {
                                        success2 = substring(queryCpy, value, 0, (size_t) spaceIndex);
                                    }
                                } else if (spaceIndex == -1) {
                                    valueLength = strlen(queryCpy);
                                    value = malloc(((size_t) valueLength + 1) * sizeof(char));
                                    if (value) {
                                        success2 = substring(queryCpy, value, 0, valueLength);
                                    }
                                }
                            }
                        }

                        if (success1 && success2 && (condition = getEmptyCondition())) {
                            key = trim(key);
                            value = trim(value);
                            condition->key = key;
                            condition->value = value;
                            condition->comparator = currentComparator;
                            tmp = realloc(c->conditions, sizeof(Condition) * (c->conditionsNumber + 1));
                            if (tmp) {
                                c->conditions = tmp;
                                c->conditions[c->conditionsNumber++] = *condition;
                            }

                        } else {
                            if (key) free(key);
                            if (value) free(value);
                        }
                    }
                } while((logicalOperatorIndex = getLogicalOperator(queryCpy, &currentLogicalOperator)) > 0);
            }

            free(savePtrPosition);
            return c;
        }
    }

    return NULL;
}

Orders *getOrders(char *query) {
    if (query) {
        int isAsc;
        int isDesc;
        long commaIndex = -1;
        long orderByIndex;
        long spaceIndex;
        char *col;
        char *queryCpy = strdup(query);
        char *savePtrPos = queryCpy;
        Orders *orders = NULL;
        Order *order = NULL;
        Order *tmp = NULL;

        if (queryCpy) {
            orders = getEmptyOrders();
            queryCpy = trim(queryCpy);
            orderByIndex = getSubstringIndex(queryCpy, "ORDER BY", 1);
            if (orderByIndex > 0) {
                queryCpy += orderByIndex + 8; // 8 is the length of "order by"
                queryCpy = trim(queryCpy);

                do {
                    if (commaIndex > 0) {
                        queryCpy += commaIndex + 1;
                        queryCpy = trim(queryCpy);
                    }

                    spaceIndex = getSubstringIndex(queryCpy, " ", 0);
                    if (spaceIndex > 1
                        && (col = malloc(sizeof(char) * (spaceIndex + 1))) && substring(queryCpy, col, 0, spaceIndex)) {
                        queryCpy += spaceIndex;
                        queryCpy = trim(queryCpy);
                        isAsc = startsWith(queryCpy, "asc", 1);
                        isDesc = startsWith(queryCpy, "desc", 1);
                        if (isAsc || isDesc) {
                            order = getEmptyOrder();
                            order->column = col;
                            order->type = isDesc ? DESC : ASC;
                            tmp = realloc(orders->orders, sizeof(Order) * (orders->ordersNumber + 1));
                            if (tmp) {
                                orders->orders = tmp;
                                orders->orders[orders->ordersNumber++] = *order;
                            } else {
                                freeOrder(order);
                                free(col);
                            }
                        } else {
                            free(col);
                        }
                    }
                } while((commaIndex = getSubstringIndex(queryCpy, ",", 0)) > 0);
            } else {
                freeOrders(orders);
                orders = NULL;
            }

            free(savePtrPos);
            return orders;
        }
    }
    return NULL;
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

                if (success2) { // remove whats after if there is
                    long index = getSubstringIndex(*column, " ", 0);
                    if (index > 0) {
                        (*column)[index] = '\0';
                    }
                }

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
        JoinType type = NONE;

        if (strCpy) {
            strCpy[index] = '\0';
            trim(strCpy);

            if (endsWith(strCpy, "left", 1)) {
                type = LEFT;
            } else if (endsWith(strCpy, "right", 1)) {
                type = RIGHT;
            } else if (endsWith(strCpy, "full", 1)) {
                type = FULL;
            } else {
                type = INNER;
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
        long indexOperator = 0;
        long indexComparator;
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
                        fieldsNumber = 0;

                        do {
                            if (indexOperator > 0) {
                                ptr += indexOperator;
                                indexSpace = getSubstringIndex(ptr, " ", 0);
                                if (indexSpace > 0) {
                                    ptr += indexSpace;
                                } else {
                                    break;
                                }
                            }
                            indexComparator = getComparator(ptr, &currentComparator);
                            if (indexComparator > 2 && getTableColumn(ptr, 0, indexComparator - 1, &table1, &col1)) { // b.b=...
                                ptr += indexComparator + 1;
                                ptr = trim(ptr);
                                if (getTableColumn(ptr, 0, strlen(ptr) - 1, &table2, &col2)) {
                                    fieldsTmp = realloc(fields, sizeof(JoinField) * ((size_t) fieldsNumber + 1));
                                    if (fieldsTmp) {
                                        fields = fieldsTmp;
                                        fields[fieldsNumber] = *getEmptyJoinField();
                                        fields[fieldsNumber].originTable = strdup(table1);
                                        fields[fieldsNumber].originColumn = strdup(col1);
                                        fields[fieldsNumber].targetTable = strdup(table2);
                                        fields[fieldsNumber].targetColumn = strdup(col2);
                                        fields[fieldsNumber].comparator = currentComparator;
                                        indexOperator = getLogicalOperator(ptr, &fields[fieldsNumber].logicOp); // This will set logicOp
                                        fieldsNumber++;
                                    }
                                }
                            }
                        } while (indexOperator > 0 && (indexOperator < getSubstringIndex(ptr, "join", 1) || getSubstringIndex(ptr, "join", 1) != -1));

                        indexOperator = -1;

                        joinsTmp = realloc(joins->joins, sizeof(Join) * ((size_t) joins->joinsNumber + 1));
                        if (joinsTmp) {
                            newJoin = getEmptyJoin();
                            newJoin->type = type;
                            newJoin->fields = fields;
                            newJoin->target = strdup(target);
                            newJoin->fieldsNumber = fieldsNumber;

                            joins->joins = joinsTmp;
                            joins->joins[joins->joinsNumber++] = *newJoin;
                        }

                        fields = NULL;
                        free(target);
                    }
                }
            }
        }

        free(queryCpy);
    }

    return joins;
}
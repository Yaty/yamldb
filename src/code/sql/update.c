//
// Created by Hugo on 16/12/2017.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../header/sql/query.h"
#include "../../header/string_array_functions.h"
#include "../../header/yaml/api.h"
#include "../../header/sql/conditions.h"
#include "../../header/sql/parser.h"

static int updateTable(char *table, Node *update, char *dbPath, Conditions *c) {
    if (table && update && dbPath) {
        char *tableDataPath = concat(4, dbPath, "/", table, "/data.yml");
        if (tableDataPath) {
            Node *rootData = YAMLParseFile(tableDataPath);
            Node *data = YAMLGetChildByKey(rootData, "data");
            Node *line;
            Node *updateLine;
            Node *column;
            int i;
            int j;
            int currentLineUpdated = 0;
            int linesUpdated = 0;

            for (i = 0; i < YAMLGetSize(data); i++) {
                line = YAMLGetChildAtIndex(data, i);
                currentLineUpdated = 0;

                if (matchConditions(c, line) || c == NULL) { // if c is NULL there is no condition so we update every line
                    for (j = 0; j < YAMLGetSize(update); j++) {
                        updateLine = YAMLGetChildAtIndex(update, j);
                        column = YAMLGetChildByKey(line, YAMLGetKey(updateLine));
                        if (column != NULL) {
                            YAMLSetValue(column, YAMLGetValue(updateLine));
                            currentLineUpdated = 1;
                        }
                    }
                }

                if (currentLineUpdated) {
                    linesUpdated++;
                }
            }

            if (data) YAMLSaveNode(data, tableDataPath);

            if (rootData) YAMLFreeNode(rootData);
            free(tableDataPath);
            return linesUpdated;
        }
    }
}

// TODO TEST UPDATE

void executeUpdate(QueryResult *res, char *query, char *dbPath) {
    if (res && query && dbPath) {
        char *queryCpy = strdup(query);
        char *savePtrPos = queryCpy;
        long spaceIndex = getSubstringIndex(queryCpy, " ", 0);
        long equalIndex;
        long commaIndex;
        char *target;
        int targetSuccess;
        int keySuccess;
        int valueSuccess;
        size_t valueLength;
        Node *data;
        char *key;
        char *value;
        Conditions *c;
        int linesUpdated = 0;
        char linesUpdatedString[12];

        if (spaceIndex > 0) {
            target = malloc(sizeof(char) * spaceIndex);
            if (target) {
                targetSuccess = substring(queryCpy, target, 0, (size_t) spaceIndex);
                if (targetSuccess) {
                    queryCpy += spaceIndex;
                    queryCpy = trim(queryCpy);
                    if (getSubstringIndex(queryCpy, "set", 1) == 0) {
                        queryCpy += 3;
                        queryCpy = trim(queryCpy);
                        data = YAMLGetMapNode("data");

                        while (1) {
                            equalIndex = getSubstringIndex(queryCpy, "=", 0);
                            if (equalIndex > 0) {
                                key = malloc(sizeof(char) * equalIndex);
                                keySuccess = substring(queryCpy, key, 0, (size_t) equalIndex);
                                if (keySuccess) {
                                    key = trim(key);
                                    queryCpy += equalIndex + 1;
                                    queryCpy = trim(queryCpy);
                                    commaIndex = getSubstringIndex(queryCpy, ",", 0);

                                    if (commaIndex == -1) {
                                        spaceIndex = getSubstringIndex(queryCpy, " ", 0);
                                        if (spaceIndex > 0) {
                                            valueLength = (size_t) spaceIndex;
                                        } else {
                                            valueLength = strlen(queryCpy);
                                        }
                                        value = malloc(sizeof(char) * valueLength);
                                        valueSuccess = substring(queryCpy, value, 0, valueLength);
                                        value = trim(value);
                                    } else {
                                        value = malloc(sizeof(char) * commaIndex);
                                        valueSuccess = substring(queryCpy, value, 0, (size_t) commaIndex);
                                        value = trim(value);
                                    }

                                    if (valueSuccess) {
                                        YAMLAddChild(data, YAMLGetValueNode(key, value));
                                    } else {
                                        break;
                                    }

                                    if (commaIndex == -1) {
                                        break;
                                    } else {
                                        queryCpy += commaIndex + 1;
                                        queryCpy = trim(queryCpy);
                                    }
                                } else {
                                    break;
                                }
                            } else {
                                break;
                            }
                        }

                        c = getConditions(queryCpy);
                        linesUpdated = updateTable(target, data, dbPath, c);
                        sprintf(linesUpdatedString, "%d", linesUpdated);
                        freeConditions(c);
                        free(target);
                        res->messagesCounter += addStringIntoArray(
                                concat(2, linesUpdatedString, " lines updated."),
                                &res->messages,
                                res->messagesCounter
                        );
                    }
                }
            }
        }

        free(savePtrPos);
    }
}
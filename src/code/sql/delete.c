//
// Created by Hugo on 15/12/2017.
//

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "../../header/sql/query.h"
#include "../../header/string_array_functions.h"
#include "../../header/sql/conditions.h"
#include "../../header/sql/parser.h"
#include "../../header/yaml/api.h"

static void setMetaDataEmptyTable(char *dbPath, char *table) {
    if (dbPath && table) {
        int i;
        long slashIndex = 0;
        char *dbPathCpy = strdup(dbPath);
        char *ptrSavePos = dbPathCpy;

        while ((slashIndex = getSubstringIndex(dbPathCpy, "/", 0)) >= 0) {
            dbPathCpy += slashIndex + 1;
        }

        char *tableMetaPath = concat(3, "./resources/", toLowerCase(dbPathCpy), ".yml");
        if (tableMetaPath) {
            Node *baseRoot = YAMLParseFile(tableMetaPath);
            Node *base = YAMLGetChildByKey(baseRoot, "tables");
            if (base) {
                Node *currentTable;
                for (i = 0; i < YAMLGetSize(base); i++) {
                    currentTable = YAMLGetChildAtIndex(base, i);
                    if (areStringsEquals(YAMLGetValue(YAMLGetChildByKey(currentTable, "name")), table, 1)) {
                        YAMLSetValue(YAMLGetChildByKey(currentTable, "empty"), strdup("yes"));
                    }
                }

                YAMLSaveNode(base, tableMetaPath);
            }
            if (baseRoot) YAMLFreeNode(baseRoot);
            free(tableMetaPath);
        }

        free(ptrSavePos);
    }
}

static int deleteLinesInTable(char *dbPath, char *table, Conditions *c) {
    if (dbPath && table) {
        char *tableDataPath = concat(4, dbPath, "/", table, "/data.yml");

        if (tableDataPath) {
            Node *dataRoot = YAMLParseFile(tableDataPath);
            Node *data = YAMLGetChildByKey(dataRoot, "data");
            int size = YAMLGetSize(data);

            if (c == NULL || size == 0) {
                setMetaDataEmptyTable(dbPath, table);
                YAMLSaveNode(YAMLGetMapNode("data"), tableDataPath);
                free(tableDataPath);
                YAMLFreeNode(dataRoot);
                return size;
            }

            int i;
            int deletedLines = 0;

            for (i = 0; i < YAMLGetSize(data); i++) {
                if (matchConditions(c, YAMLGetChildAtIndex(data, i))) {
                    i -= YAMLRemoveChildAtIndex(data, i); // make i stay at this index to eval then next line
                    deletedLines++;
                }
            }

            if (YAMLGetSize(data) == 0) {
                setMetaDataEmptyTable(dbPath, table);
            }

            if (data) YAMLSaveNode(data, tableDataPath);
            if (dataRoot) YAMLFreeNode(dataRoot);
            free(tableDataPath);
            return deletedLines;
        }
    }

    return 0;
}

static void setZeroLineMsg(QueryResult *res) {
    res->messagesCounter += addStringIntoArray(
            strdup("0 line deleted."),
            &res->messages,
            res->messagesCounter
    );
}

/**
 * Execute a delete SQL query
 * @param query
 * @param dbPath path to the db yaml file
 * @return a query result
 */
void executeDelete(QueryResult *res, char *query, char *dbPath) {
    if (res && query && dbPath) {
        char *queryCpy = strdup(query);
        char *ptrSavePos = queryCpy;
        queryCpy = trim(queryCpy);
        long spaceIndex = getSubstringIndex(queryCpy, " ", 0);
        char *target;
        int successTarget;
        int deletedLines;
        char deletedLines2[12];
        Conditions *c = NULL;

        spaceIndex = spaceIndex == -1 ? strlen(queryCpy) : (size_t) spaceIndex;
        if (spaceIndex > 0) {
            target = malloc(sizeof(char) * spaceIndex);
            if (target) {
                successTarget = substring(queryCpy, target, 0, (size_t) spaceIndex);
                c = getConditions(queryCpy);
                if (successTarget) {
                    deletedLines = deleteLinesInTable(dbPath, target, c);
                    sprintf(deletedLines2, "%d", deletedLines); // string to int
                    if (c) freeConditions(c);
                    res->messagesCounter += addStringIntoArray(
                            concat(2, deletedLines2, " lines deleted."),
                            &res->messages,
                            res->messagesCounter
                    );
                } else {
                    setZeroLineMsg(res);
                }
            } else {
                setZeroLineMsg(res);
            }
        } else {
            setZeroLineMsg(res);
        }

        free(ptrSavePos);
    }
}
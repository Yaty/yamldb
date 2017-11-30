//
// Created by Hugo on 12/11/2017.
//

#include <string.h>
#include <stdlib.h>
#include "../../header/yaml/api.h"
#include "../../header/sql/query.h"
#include "../../header/string_array_functions.h"

/**
 * Add a warning message to a query result
 * @param result
 * @param warning
 * @return 1 for success, 0 for failure
 */
int addWarningToResult(QueryResult *result, char *warning) {
    if (warning) {
        char **tmp = realloc(result->warnings, result->warningsCounter + 1);
        if (tmp) {
            result->warnings = tmp;
            result->warnings[result->warningsCounter++] = warning;
            return 1;
        }
    }

    return 0;
}

/**
 * Load data and metadata from a table
 * @param dbPath
 * @param currentTable
 * @param data
 * @param metas
 * @return the number of lines
 */
int loadData(char *dbPath, char *currentTable, Node **data, Node **metas) {
    char *currentTablePath = concat(3, dbPath, "/", currentTable);
    char *currentTableMetaPath = concat(2, currentTablePath, "/metadata.yml");
    char *currentTableDataPath = concat(2, currentTablePath, "/data.yml");
    Node *rootMetas = YAMLParseFile(currentTableMetaPath);
    Node *rootData = YAMLParseFile(currentTableDataPath);

    *metas = YAMLGetChildByKey(rootMetas, "structure");
    *data = YAMLGetChildByKey(rootData, "data");

    free(currentTableDataPath);
    free(currentTableMetaPath);
    free(currentTablePath);

    if (*metas && *data) {
        YAMLPartialNodeFree(rootData);
        YAMLPartialNodeFree(rootMetas);
        return YAMLGetSize(*data);
    } else {
        YAMLFreeNode(rootData);
        YAMLFreeNode(rootMetas);
        return 0;
    }
}

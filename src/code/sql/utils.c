//
// Created by Hugo on 12/11/2017.
//

#include <string.h>
#include <stdlib.h>
#include "../../header/yaml/api.h"
#include "../../header/sql/query.h"
#include "../../header/string_array_functions.h"
#include "../../header/utils/hashmap.h"

/**
 * Add a warning message to a query result
 * @param result
 * @param warning
 * @return 1 for success, 0 for failure
 */
int addWarningToResult(QueryResult *result, char *warning) {
    if (warning) {
        return addStringIntoArray(warning, &result->warnings, (int) result->warningsCounter);
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

/**
 * Add a node by his file path to a hashmap
 * Data key = table
 * Metadata key = table-metadata
 * @param dbPath
 * @param table
 * @param data
 */
void addNodeToHashMap(char *dbPath, char *table, HashMap *data) {
    char *currentNodeDataPath = concat(4, dbPath, "/", table, "/data.yml");
    Node *currentNodeRoot = YAMLParseFile(currentNodeDataPath);
    Node *currentNode = YAMLGetChildByKey(currentNodeRoot, "data");

    char *currentNodeDataPathMeta = concat(4, dbPath, "/", table, "/metadata.yml");
    Node *currentNodeRootMeta = YAMLParseFile(currentNodeDataPathMeta);
    Node *currentNodeMeta = YAMLGetChildByKey(currentNodeRootMeta, "structure");

    if (DBIsValidData(currentNode) && DBIsValidMetadata(currentNodeMeta)) {
        hashInsert(data, strdup(table), currentNode);
        hashInsert(data, concat(2, table, "-metadata"), currentNodeMeta);
        YAMLPartialNodeFree(currentNodeRoot);
        YAMLPartialNodeFree(currentNodeRootMeta);
    } else {
        YAMLFreeNode(currentNodeRoot);
        YAMLFreeNode(currentNodeRootMeta);
    }

    free(currentNodeDataPath);
    free(currentNodeDataPathMeta);
}

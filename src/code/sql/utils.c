//
// Created by Hugo on 12/11/2017.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../header/yaml/api.h"
#include "../../header/sql/query.h"
#include "../../header/string_array_functions.h"
#include "../../header/utils/hashmap.h"
#include "../../header/sql/join.h"

/**
 * Add a warning message to a query result
 * @param result
 * @param warning
 * @return 1 for success, 0 for failure
 */
int addWarningToResult(QueryResult *result, char *warning) {
    if (warning) {
        int res = addStringIntoArray(warning, &result->warnings, (int) result->warningsCounter);
        result->warningsCounter += res;
        return res;
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

/**
 * Free hash map which is filled by node struct
 * @param map
 */
void freeHashMapFilledWithNode(HashMap *map) {
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
HashMap *initDataMap(Joins *joins, char **tables, int tablesCounter, char *dbPath) {
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
            if (hashLookup(data, tables[i]) == NULL) { // do not add twice
                addNodeToHashMap(dbPath, tables[i], data);
            }
        }

        return data;

    }

    return hashNew(0);
}

/**
 * Get meta node from a hashmap
 * @param dataMap
 * @param table
 * @return
 */
Node *getMetas(HashMap *dataMap, char *table) {
    if (dataMap && table) {
        char *path = concat(2, table, "-metadata");
        Node *metas = hashLookup(dataMap, path);
        free(path);
        return metas;
    }

    return NULL;
}

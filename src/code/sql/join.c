//
// Created by Hugo on 27/11/2017.
//

#include <stdlib.h>
#include <string.h>
#include "../../header/sql/join.h"
#include "../../header/yaml/api.h"
#include "../../header/string_array_functions.h"
#include "../../header/sql/parser.h"

/**
 * Get empty initialized Joins
 * @return a Joins
 */
Joins getEmptyJoins() {
    Joins j;
    j.joinsNumber = 0;
    j.joins = NULL;
    return j;
}

/**
 * Get empty initialized join
 * @return a Join
 */
Join getEmptyJoin() {
    Join j;
    j.type = NONE;
    j.fieldsNumber = 0;
    j.fields = NULL;
    j.target = NULL;
    return j;
}

/**
 * Get empty initialized join field
 * @return a JoinField
 */
JoinField getEmptyJoinField() {
    JoinField j;
    j.comparator = NO_COMPARATOR;
    j.originTable = NULL;
    j.originColumn = NULL;
    j.targetTable = NULL;
    j.targetColumn = NULL;
    j.logicOp = NO_OPERATOR;
    return j;
}

/**
 * Return a HashMap filled with data Nodes
 * Key : table name
 * Value : the yml file parsed, browsable in a Node struct
 * @param joins
 * @param dbPath
 * @return the hashmap
 */
HashMap *initDataMap(Joins *joins, char *dbPath) {
    if (joins) {
        int i;
        HashMap *data = hashNew(joins->joinsNumber);
        Join currentJoin;
        Node *currentNodeRoot;
        Node *currentNode;
        char *currentNodeDataPath;

        for (i = 0; i < joins->joinsNumber; i++) {
            currentJoin = joins->joins[i];
            currentNodeDataPath = concat(3, dbPath, "/", currentJoin.target);
            currentNodeRoot = YAMLParseFile(currentNodeDataPath);
            currentNode = YAMLGetChildByKey(currentNodeRoot, "data");
            if (DBIsValidData(currentNode)) {
                hashInsert(data, currentJoin.target, currentNode);
                YAMLPartialNodeFree(currentNodeRoot);
            } else {
                YAMLFreeNode(currentNodeRoot);
            }
            free(currentNodeDataPath);
        }

        return data;
    }

    return hashNew(0);
}

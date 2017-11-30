//
// Created by Hugo on 27/11/2017.
//

#ifndef CBDD1_JOIN_H
#define CBDD1_JOIN_H

#include "../utils/hashmap.h"
#include "operators.h"

typedef enum JoinType {
    NONE, INNER, LEFT, RIGHT, FULL
} JoinType;

typedef struct JoinField {
    char *originTable;
    char *originColumn;
    char *targetTable;
    char *targetColumn;
    Comparator comparator;
    LogicalOperator logicOp;
} JoinField;

typedef struct Join {
    enum JoinType type;
    JoinField *fields;
    int fieldsNumber;
    char *target;
} Join;

typedef struct Joins {
    Join *joins;
    int joinsNumber;
} Joins;

/**
 * Get empty initialized Joins
 * @return a Joins
 */
Joins getEmptyJoins();

/**
 * Get empty initialized join
 * @return a Join
 */
Join getEmptyJoin();

/**
 * Get empty initialized join field
 * @return a JoinField
 */
JoinField getEmptyJoinField();

/**
 * Return a HashMap filled with data Nodes
 * Key : table name
 * Value : the yml file parsed, browsable in a Node struct
 * @param joins
 * @param dbPath
 * @return the hashmap
 */
HashMap *initDataMap(Joins *joins, char *dbPath);

#endif //CBDD1_JOIN_H

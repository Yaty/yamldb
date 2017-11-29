//
// Created by Hugo on 27/11/2017.
//

#ifndef CBDD1_JOIN_H
#define CBDD1_JOIN_H

typedef enum JoinType {
    NONE, INNER, LEFT, RIGHT, FULL
} JoinType;

typedef enum Comparator {
    NO_COMPARATOR, EQUAL, GREATER, LESSER, GREATER_EQUAL, LESSER_EQUAL, NOT_EQUAL
} Comparator;

typedef enum LogicalOperator {
    NO_OPERATOR, AND, OR, ANY, BETWEEN, EXISTS, IN, LIKE, NOT, SOME
} LogicalOperator;

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
} Join;

typedef struct Joins {
    char *target;
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

#endif //CBDD1_JOIN_H

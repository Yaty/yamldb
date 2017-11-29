//
// Created by Hugo on 27/11/2017.
//

#include <stdlib.h>
#include "../../header/sql/join.h"

/**
 * Get empty initialized Joins
 * @return a Joins
 */
Joins getEmptyJoins() {
    Joins j;
    j.joinsNumber = 0;
    j.target = NULL;
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
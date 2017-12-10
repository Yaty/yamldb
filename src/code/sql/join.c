//
// Created by Hugo on 27/11/2017.
//

#include <stdlib.h>
#include <stdio.h>
#include "../../header/sql/join.h"

/**
 * Get empty initialized Joins
 * @return a Joins
 */
Joins *getEmptyJoins() {
    Joins *j = malloc(sizeof(Joins));
    j->joinsNumber = 0;
    j->joins = NULL;
    return j;
}

/**
 * Get empty initialized join
 * @return a Join
 */
Join *getEmptyJoin() {
    Join *j = malloc(sizeof(Join));
    j->type = NONE;
    j->fieldsNumber = 0;
    j->fields = NULL;
    j->target = NULL;
    return j;
}

/**
 * Get empty initialized join field
 * @return a JoinField
 */
JoinField *getEmptyJoinField() {
    JoinField *j = malloc(sizeof(JoinField));
    j->comparator = NO_COMPARATOR;
    j->originTable = NULL;
    j->originColumn = NULL;
    j->targetTable = NULL;
    j->targetColumn = NULL;
    j->logicOp = NO_OPERATOR;
    return j;
}

/**
 * Free join field
 * @param field
 */
void freeJoinField(JoinField *field) {
    if (field) {
        if (field->originTable) {
            free(field->originTable);
            field->originTable = NULL;
        }

        if (field->originColumn) {
            free(field->originColumn);
            field->originColumn = NULL;
        }

        if (field->targetTable) {
            free(field->targetTable);
            field->targetTable = NULL;
        }

        if (field->targetColumn) {
            free(field->targetColumn);
            field->targetColumn = NULL;
        }

        /*
        free(field);
        field = NULL;
         */
    }
}

/**
 * Free join
 * @param join
 */
void freeJoin(Join *join) {
    if (join) {
        int i;

        if (join->target) {
            free(join->target);
            join->target = NULL;
        }

        for (i = 0; i < join->fieldsNumber; i++) {
            freeJoinField(&join->fields[i]);
        }

        /*
        free(join);
        join = NULL;
         */
    }
}

/**
 * Free joins
 * @param joins
 */
void freeJoins(Joins *joins) {
    if (joins) {
        int i;

        for (i = 0; i < joins->joinsNumber; i++) {
            freeJoin(&joins->joins[i]);
        }

        free(joins);
        joins = NULL;
    }
}

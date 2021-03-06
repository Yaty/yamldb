//
// Created by Hugo on 13/12/2017.
//

#ifndef CBDD1_CONDITIONS_H
#define CBDD1_CONDITIONS_H

#include "operators.h"
#include "../yaml/node.h"

typedef struct Condition {
    char *key;
    char *value;
    Comparator comparator;
} Condition;

typedef struct Conditions {
    Condition *conditions;
    int conditionsNumber;
    int operatorsNumber;
    LogicalOperator *operators;
} Conditions;

/**
 * Get an empty conditions struct ready to use
 * @return conditions
 */
Conditions *getEmptyConditions();

/**
 * Get an empty condition struct ready to use
 * @return condition
 */
Condition *getEmptyCondition();

/**
 * Free a conditions struct
 * @param c the struct to free
 */
void freeConditions(Conditions *c);


/**
 * See if line match conditions, if true 1 if false 0
 * @param c
 * @param line
 * @return
 */
int matchConditions(Conditions *c, Node *line);


#endif //CBDD1_CONDITIONS_H

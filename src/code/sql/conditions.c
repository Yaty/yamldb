//
// Created by Hugo on 13/12/2017.
//

#include <stdlib.h>
#include "../../header/sql/conditions.h"

/**
 * Get an empty condition struct ready to use
 * @return condition
 */
Condition *getEmptyCondition() {
    Condition *condition = malloc(sizeof(Condition));
    condition->key = NULL;
    condition->value = NULL;
    return condition;
}

/**
 * Get an empty conditions struct ready to use
 * @return conditions
 */
Conditions *getEmptyConditions() {
    Conditions *conditions = malloc(sizeof(Conditions));
    conditions->conditionsNumber = 0;
    conditions->conditions = NULL;
    conditions->operatorsNumber = 0;
    conditions->operators = NULL;
    return conditions;
}

/**
 * Free a conditions struct
 * @param c the struct to free
 */
void freeConditions(Conditions *c) {
    if (c) {
        int i;
        Condition *condition;
        for (i = 0; i < c->conditionsNumber; i++) {
            condition = &(c->conditions[i]);

            if (condition->key) {
                free(condition->key);
                condition->key = NULL;
            }

            if (condition->value) {
                free(condition->value);
                condition->value = NULL;
            }

            free(condition);
            condition = NULL;
        }

        free(c);
        c = NULL;
    }
}
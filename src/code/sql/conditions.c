//
// Created by Hugo on 13/12/2017.
//

#include <stdlib.h>
#include <memory.h>
#include "../../header/sql/conditions.h"
#include "../../header/yaml/api.h"
#include "../../header/string_array_functions.h"

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

/**
 * See if line match conditions, if true 1 if false 0
 * @param c
 * @param line
 * @return
 */
int matchConditions(Conditions *c, Node *line) {
    if (c && line && c->conditionsNumber == c->operatorsNumber + 1) {
        int i;
        int j;
        int addLine;
        int evalConditions[c->conditionsNumber];
        Condition condition;
        Type colType;

        memset(evalConditions, 0, (size_t) c->conditionsNumber * sizeof(int)); // reset to 0, as evalConditions is variable-sized we have to do this by this way


        for (i = 0; i < c->conditionsNumber; i++) {
            condition = c->conditions[i];
            colType = evalType(condition.key);

            for (j = 0; j < YAMLGetSize(line); j++) {
                if (areStringsEquals(condition.key, YAMLGetKey(YAMLGetChildAtIndex(line, j)), 1)) {
                    evalConditions[i] = evalComparator(condition.value, YAMLGetValue(YAMLGetChildAtIndex(line, j)), colType, condition.comparator);
                    break;
                }
            }
        }

        if (c->conditionsNumber == 1) {
            addLine = evalConditions[0];
        } else {
            addLine = 1;

            for (i = 0; i < c->conditionsNumber - 1; i++) {
                addLine &= evalOperatorInt(evalConditions[i], evalConditions[i + 1], c->operators[i]);
            }
        }

        return addLine;
    }

    return 0;
}
//
// Created by Hugo on 01/12/2017.
//

#include <stdlib.h>
#include "../../header/sql/operators.h"
#include "../../header/string_array_functions.h"

/**
 * Eval an integer with an operator
 * @param a
 * @param b
 * @param operator
 * @return the result of the evaluation
 */
int evalOperatorInt(int a, int b, LogicalOperator operator) {
    // ANY, BETWEEN, EXISTS, IN, LIKE, SOME : TODO
    switch (operator) {
        case AND:
            return a && b;
        case OR:
            return a || b;
        case NOT:
            return a != b;
        default:
            return 0;
    }
}

/**
 * Eval a string with a comparator
 * @param a
 * @param b
 * @param operator
 * @return the comparator result
 */
int evalComparatorString(char *a, char *b, Comparator operator) {
    switch (operator) {
        case EQUAL:
            return areStringsEquals(a, b, 0);
        case NOT_EQUAL:
            return !areStringsEquals(a, b, 0);
        default:
            return 0;
    }
}

/**
 * Eval an int with a comparator
 * @param a
 * @param b
 * @param operator
 * @return the comparator result
 */
int evalComparatorInt(int a, int b, Comparator operator) {
    switch (operator) {
        case EQUAL:
            return a == b;
        case GREATER:
            return a > b;
        case LESSER:
            return a < b;
        case GREATER_EQUAL:
            return a >= b;
        case LESSER_EQUAL:
            return a <= b;
        case NOT_EQUAL:
            return a != b;
        default:
            return 0;
    }
};

/**
 * Check if two values are equals according to a type
 * @param a
 * @param b
 * @param type
 * @return 1 for equal, 0 for not, -1 for error
 */
int isEqual(char *a, char *b, char *type) {
    if (a && b && type) {
        type = toLowerCase(type);
        if (areStringsEquals(type, "int", 1)) { // TODO, HANDLE ALL TYPES
            return strtol(a, NULL, 10) == strtol(b, NULL, 10);
        } else if (areStringsEquals(type, "string", 1)) {
            return areStringsEquals((char*) a, (char*) b, 0);
        }
    }

    return -1;
}
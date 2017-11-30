//
// Created by Hugo on 01/12/2017.
//

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
            return areStringsEquals(a, b);
        case NOT_EQUAL:
            return !areStringsEquals(a, b);
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
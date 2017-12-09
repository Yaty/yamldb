//
// Created by Hugo on 01/12/2017.
//

#ifndef CBDD1_OPERATORS_H
#define CBDD1_OPERATORS_H

typedef enum Comparator {
    NO_COMPARATOR, EQUAL, GREATER, LESSER, GREATER_EQUAL, LESSER_EQUAL, NOT_EQUAL
} Comparator;

typedef enum LogicalOperator {
    NO_OPERATOR, AND, OR, ANY, BETWEEN, EXISTS, IN, LIKE, NOT, SOME
} LogicalOperator;

/**
 * Eval an integer with an operator
 * @param a
 * @param b
 * @param operator
 * @return the result of the evaluation
 */
int evalOperatorInt(int a, int b, LogicalOperator operator);

/**
 * Eval a string with a comparator
 * @param a
 * @param b
 * @param operator
 * @return the comparator result
 */
int evalComparatorString(char *a, char *b, Comparator operator);

/**
 * Eval an int with a comparator
 * @param a
 * @param b
 * @param operator
 * @return the comparator result
 */
int evalComparatorInt(int a, int b, Comparator operator);

/**
 * Check if two values are equals according to a type
 * @param a
 * @param b
 * @param type
 * @return 1 for equal, 0 for not, -1 for error
 */
int isEqual(char *a, char *b, char *type);

#endif //CBDD1_OPERATORS_H

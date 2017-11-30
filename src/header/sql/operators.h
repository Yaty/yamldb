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

#endif //CBDD1_OPERATORS_H

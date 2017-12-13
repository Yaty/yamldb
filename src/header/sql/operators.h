//
// Created by Hugo on 01/12/2017.
//

#ifndef CBDD1_OPERATORS_H
#define CBDD1_OPERATORS_H

#include "../utils/type.h"

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

int evalComparator(char *a, char *b, Type type, Comparator comparator);

/**
 * Eval a string with a comparator
 * @param a
 * @param b
 * @param operator
 * @return the comparator result
 */
int evalComparatorString(char *a, char *b, Comparator operator);

/**
 * Eval a double with a comparator
 * @param a
 * @param b
 * @param operator
 * @return the comparator result
 */
int evalComparatorDouble(double a, double b, Comparator operator);

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

/**
 * Check if value a is greater than value b according to a type
 * @param a
 * @param b
 * @param type
 * @return 1 for equal, 0 for not, -1 for error
 */
int isGreater(char *a, char *b, char *type);

/**
 * Check if value a is lesser than value b according to a type
 * @param a
 * @param b
 * @param type
 * @return 1 for equal, 0 for not, -1 for error
 */
int isLesser(char *a, char *b, char *type);

/**
 * Check if value a is greater of equal to value b according to a type
 * @param a
 * @param b
 * @param type
 * @return 1 for equal, 0 for not, -1 for error
 */
int isGreaterOrEqual(char *a, char *b, char *type);

/**
 * Check if value a is lesser of equal to value b according to a type
 * @param a
 * @param b
 * @param type
 * @return 1 for equal, 0 for not, -1 for error
 */
int isLesserOrEqual(char *a, char *b, char *type);

/**
 * Check if value a not equal to value b according to a type
 * @param a
 * @param b
 * @param type
 * @return 1 for equal, 0 for not, -1 for error
 */
int isNotEqual(char *a, char *b, char *type);

#endif //CBDD1_OPERATORS_H

//
// Created by Hugo on 13/12/2017.
//

#ifndef CBDD1_ORDER_H
#define CBDD1_ORDER_H

#include "query.h"
#include "operators.h"

typedef enum OrderType {
    ASC, DESC
} OrderType;

typedef struct Order {
    char *column;
    OrderType type;
} Order;

typedef struct Orders {
    Order *orders;
    int ordersNumber;
} Orders;

typedef struct SortParameters {
    int columnIndex;
    OrderType orderType;
} SortParameters;

/**
 * Get allocated order
 * @return order
 */
Order *getEmptyOrder();

/**
 * Get allocated orders
 * @return orders
 */
Orders *getEmptyOrders();

/**
 * Free an order
 * @param o
 */
void freeOrder(Order *o);


/**
 * Free orders
 * @param o
 */
void freeOrders(Orders *o);

/**
 * Order a query result from an orders struct
 * @param res
 * @param o
 */
void makeOrder(QueryResult *res, Orders *o);

/**
 * Sort a table with a merge sort algorithm
 * @param table
 * @param l left index (0 if you want to sort from the start)
 * @param r right index (tables length - 1 if you want to sort til the array end)
 * @param sp additional parameters (orderType, column to sort on)
 */
void mergeSort(char ***table, int l, int r, SortParameters sp);

/**
 * Function to merge the two haves table[l..m] and table[m+1..r] of array table[]
 * @param table
 * @param l left index
 * @param m middle index
 * @param r right index
 * @param sp additional parameters (orderType, column to sort on)
 */
void merge(char ***table, int l, int m, int r, SortParameters sp);

#endif //CBDD1_ORDER_H

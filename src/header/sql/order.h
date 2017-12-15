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

void makeOrder(QueryResult *res, Orders *o);

void mergeSort(char ***table, int l, int r, SortParameters sp);

void merge(char ***table, int l, int m, int r, SortParameters sp);

#endif //CBDD1_ORDER_H

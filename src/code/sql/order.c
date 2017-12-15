//
// Created by Hugo on 13/12/2017.
//

#include <stdlib.h>
#include <stdio.h>
#include "../../header/sql/order.h"
#include "../../header/string_array_functions.h"
#include "../../header/utils/type.h"
#include "../../header/sql/operators.h"

/**
 * Get allocated order
 * @return order
 */
Order *getEmptyOrder() {
    Order *order = malloc(sizeof(Order));
    order->column = NULL;
    order->type = ASC;
    return order;
}

/**
 * Get allocated orders
 * @return orders
 */
Orders *getEmptyOrders() {
    Orders *orders = malloc(sizeof(Orders));
    orders->orders = NULL;
    orders->ordersNumber = 0;
    return orders;
}

/**
 * Free an order
 * @param o
 */
void freeOrder(Order *o) {
    if (o) {
        if (o->column) {
           free(o->column);
           o->column = NULL;
        }

        free(o);
    }
}

/**
 * Free orders
 * @param o
 */
void freeOrders(Orders *o) {
    if (o) {
        int i;

        for (i = 0; i < o->ordersNumber; o++) {
            freeOrder(&o->orders[i]);
        }

        if (o->orders) {
            free(o->orders);
        }

        free(o);
    }
}

// We use a merge sort because it's a stable sort, if lines are equals their order will not be changed
// Furthermore it's a O(n log n) algorithm, which is nice !
// Origin sources : http://www.geeksforgeeks.org/iterative-merge-sort/

/**
 * Sort a table with a merge sort algorithm
 * @param table
 * @param l left index (0 if you want to sort from the start)
 * @param r right index (tables length - 1 if you want to sort til the array end)
 * @param sp additional parameters (orderType, column to sort on)
 */
void mergeSort(char ***table, int l, int r, SortParameters sp) {
    if (l < r) {
        int m = l+(r-l)/2; //Same as (l+r)/2 but avoids overflow for large l & h
        mergeSort(table, l, m, sp);
        mergeSort(table, m+1, r, sp);
        merge(table, l, m, r, sp);
    }
}

/**
 * Function to merge the two haves table[l..m] and table[m+1..r] of array table[]
 * @param table
 * @param l left index
 * @param m middle index
 * @param r right index
 * @param sp additional parameters (orderType, column to sort on)
 */
void merge(char ***table, int l, int m, int r, SortParameters sp) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    char *value1, *value2;
    Type valuesType;

    /* create temp arrays */
    char ***L = malloc(sizeof(char**) * n1);
    char ***R = malloc(sizeof(char**) * n2);

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = table[l + i];
    for (j = 0; j < n2; j++)
        R[j] = table[m + 1 + j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        value1 = L[i][sp.columnIndex];
        value2 = R[j][sp.columnIndex];
        valuesType = evalType(value1);

        // we suppose they have the same type, if its not the case then the sort will be incorrect
        // but that's what the user wanted so ...
        if (valuesType != UNKNOWN && value1 && value2) {
            int res = evalComparator(value1, value2, valuesType, LESSER_EQUAL);
            if (res && sp.orderType == ASC) {
                table[k] = L[i];
                i++;
            } else {
                table[k] = R[j];
                j++;
            }
            k++;
        }
    }

    /* Copy the remaining elements of L[], if there are any */
    while (i < n1)
    {
        table[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there are any */
    while (j < n2)
    {
        table[k] = R[j];
        j++;
        k++;
    }
}

/**
 * Order a query result from an orders struct
 * @param res
 * @param o
 */
void makeOrder(QueryResult *res, Orders *o) {
    if (res && res->rowsCounter > 1 && o) {
        int i;
        int j;
        int columnSortedIndex;
        SortParameters sortParameters;

        for (i = 0; i < o->ordersNumber; i++) {
            columnSortedIndex = -1;

            for (j = 0; j < res->columnsCounter; j++) {
                if (areStringsEquals(res->headers[j], o->orders[i].column, 1)) {
                    columnSortedIndex = j;
                    break;
                }
            }

            if (columnSortedIndex == -1) { // do not sort a line without a proper column to sort on
                break;
            }

            sortParameters.columnIndex = columnSortedIndex;
            sortParameters.orderType = o->orders[i].type;
            mergeSort(res->table, 0, res->rowsCounter - 1, sortParameters);
        }
    }
}

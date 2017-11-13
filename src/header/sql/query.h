//
// Created by Hugo on 11/11/2017.
//

#ifndef CBDD1_QUERY_H
#define CBDD1_QUERY_H

#include "structs.h"

QueryResult SQLExecuteQuery(char *query);

void SQLFreeQueryResult(QueryResult *res);

#endif //CBDD1_QUERY_H

//
// Created by Hugo on 12/11/2017.
//

#ifndef CBDD1_STRUCTS_H
#define CBDD1_STRUCTS_H

#include <strings.h>

typedef enum QueryStatus { SUCCESS, FAILURE } QueryStatus;

typedef struct QueryResult {
    char ***lines;
    size_t linesCounter;
    size_t rowsCounter;
    QueryStatus status;
    char *message;
} QueryResult;

#endif //CBDD1_STRUCTS_H

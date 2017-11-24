//
// Created by Hugo on 12/11/2017.
//

#ifndef CBDD1_STRUCTS_H
#define CBDD1_STRUCTS_H

#include <strings.h>

typedef enum QueryStatus { SUCCESS, FAILURE } QueryStatus;

typedef struct QueryResult {
    char ***table;
    char **headers;
    size_t columnsCounter;
    size_t rowsCounter;
    QueryStatus status;
    char *message;
    char **warnings;
    size_t warningsCounter;
} QueryResult;

#endif //CBDD1_STRUCTS_H

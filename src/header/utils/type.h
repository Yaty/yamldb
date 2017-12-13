//
// Created by Hugo on 13/12/2017.
//

#ifndef CBDD1_UTILS_TYPE_H
#define CBDD1_UTILS_TYPE_H

typedef enum Type {
    UNKNOWN, CHAR, STRING, INT, DOUBLE
} Type;

Type evalType(char *str);

#endif //CBDD1_UTILS_TYPE_H

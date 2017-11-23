//
// Created by Hugo on 23/11/2017.
//

#ifndef CBDD1_DB_H
#define CBDD1_DB_H

#include "../node.h"

/**
 * Check if a node is a valid metadata file for storing table informations
 * @param node
 * @return 1 if true, 0 false
 */
int DBIsValidMetadata(Node *node);

/**
 * Check if a string is a valid column type
 * @param type
 * @return 1 for true, 0 for false
 */
int DBIsValidColumnType(char *type);

#endif //CBDD1_DB_H

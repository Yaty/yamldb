//
// Created by Hugo on 01/11/2017.
//

#ifndef CBDD1_API_H
#define CBDD1_API_H

#include "node.h"

/**
 * Parse a YAML file
 * @param path to the YAML file
 * @return a Node struct representing the YAML
 */
Node *YAMLParseFile(char* path);

/**
 * Print a Node struct
 * @param node the node to print
 * @param depth initialize with 0 or -1 this value handle indentations
 */
void YAMLNodeToString (Node *node, int depth);

/**
 * Free recursively a Node and his children
 * @param node the node to free
 */
void YAMLFreeNode (Node *node);

#endif //CBDD1_API_H

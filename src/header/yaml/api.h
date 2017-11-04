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
Node *YAMLParseFile (char *path);

/**
 * Print a node in the console
 * @param node
 */
void YAMLPrintNode (Node *node);

/**
 * Print a node in a file
 * @param node
 * @param path file path, it will overwrite
 * @return 1 for succes, 0 for failure
 */
int YAMLSaveNode (Node *node, char *path);

/**
 * Free recursively a Node and his children
 * @param node the node to free
 */
void YAMLFreeNode (Node *node);

#endif //CBDD1_API_H

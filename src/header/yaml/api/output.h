//
// Created by Hugo on 05/11/2017.
//

#ifndef CBDD1_OUTPUT_H
#define CBDD1_OUTPUT_H


/**
 * Save a node into a file
 * @param node
 * @param path
 * @return 1 for succes, 0 otherwise
 */
int YAMLSaveNode(Node *node, char *path);

/**
 * Print a node in the console
 * @param node
 */
void YAMLPrintNode (Node *node);

#endif //CBDD1_OUTPUT_H

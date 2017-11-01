//
// Created by Hugo on 01/11/2017.
//

#ifndef CBDD1_API_H
#define CBDD1_API_H

#include "node.h"

/**
 * Print in a file arguments if defined
 * Print in console otherwise
 * @param file the file to write to if defined
 * @param argv the number of arguments
 * @param ... the agumentes
 */
static void print (FILE* file, int argv, ...);

/**
 * Browse a node and his childs to print them in a YAML way
 * @param node the root node to print
 * @param depth set to 0
 * @param file if specified it will print in that file
 */
static void output (Node *node, int depth, FILE* file);

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

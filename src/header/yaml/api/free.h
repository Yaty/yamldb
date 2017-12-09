//
// Created by Hugo on 05/11/2017.
//

#ifndef CBDD1_FREE_YAML_H
#define CBDD1_FREE_YAML_H

/**
 * Free recursively a Node and his children
 * @param node the node to free
 * @return 1 if all children were freed, 0 otherwise
 */
int YAMLFreeNode (Node *node);

/**
 * Only free a node without freeing his children
 * @param node
 * @return 1 for success, 0 for failure
 */
int YAMLPartialNodeFree(Node *node);

#endif //CBDD1_FREE_YAML_H

//
// Created by Hugo on 05/11/2017.
//

#ifndef CBDD1_FREE_H
#define CBDD1_FREE_H

/**
 * Free recursively a Node and his children
 * @param node the node to free
 * @return 1 if all children were freed, 0 otherwise
 */
int YAMLFreeNode (Node *node);

#endif //CBDD1_FREE_H

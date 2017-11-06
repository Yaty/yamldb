//
// Created by Hugo on 05/11/2017.
//

#include <stdlib.h>
// #include <stdio.h>
#include "../../../header/yaml/node.h"

/**
 * Free recursively a Node and his children
 * @param node the node to free
 * @return 1 if all children were freed, 0 otherwise
 */
int YAMLFreeNode (Node *node) {
    if (node) {
        int i;
        int success = 1;

        for (i = 0; i < node->childrenNumber; i++) {
            if (!YAMLFreeNode(&(node->children[i]))) {
                success = 0;
            }
        }

        /*
        if (node->children) {
            free(node->children);
            node->children = NULL;
        }
        */

        if (node->key) {
            free(node->key);
            node->key = NULL;
        }

        if (node->value) {
            free(node->value);
            node->value = NULL;
        }

        free(node);
        node = NULL;

        return success;
    }

    return 0;
}


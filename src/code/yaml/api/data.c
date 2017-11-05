//
// Created by Hugo on 05/11/2017.
//

#include <stdlib.h>
#include <string.h>
#include "../../../header/yaml/node.h"

/**
 * NODE DATA
 */

/**
 * Get Node Value
 * @param node
 * @return the value if she exists, NULL otherwise
 */
char *YAMLGetValue(Node *node) {
    if (node) {
        return node->value;
    }

    return NULL;
}

/**
 * Set Node value
 * @param node
 * @param value
 * @return 1 for success, 0 otherwise
 */
int YAMLSetValue(Node *node, char *value) {
    if (node && value) {
        free(node->value);
        node->value = strdup(value);
        if (node->value) {
            return 1;
        }
    }

    return 0;
}

/**
 * Get Node key
 * @param node
 * @return a key if she exists, NULL otherwise
 */
char *YAMLGetKey(Node *node) {
    if (node) {
        return node->key;
    }

    return NULL;
}

/**
 * Set Node key
 * @param node
 * @param key
 * @return 1 for success, 0 otherwise
 */
int YAMLSetKey(Node *node, char *key) {
    if (node && key) {
        free(node->key);
        node->key = strdup(key);
        if (node->key) {
            return 1;
        }
    }

    return 0;
}

/**
 * Get Node ID
 * @param node
 * @return an ID, -1 if there is none
 */
int YAMLGetId(Node *node) {
    if (node) {
        return node->id;
    }

    return -1;
}

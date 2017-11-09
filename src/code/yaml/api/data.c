//
// Created by Hugo on 05/11/2017.
//

#include <stdlib.h>
#include <string.h>
#include "../../../header/yaml/node.h"
#include "../../../header/yaml/utils/parser.h"

/**
 * NODE DATA
 */

/**
 * Return an empty map node
 * @return a node
 */
Node *YAMLGetMapNode() {
    Node *node = getEmptyNode();
    if (node) {
        node->type = MAP;
        return node;
    }
    return NULL;
}

/**
 * Return an empty sequence node
 * @return a node
 */
Node *YAMLGetSequenceNode() {
    Node *node = getEmptyNode();
    if (node) {
        node->type = SEQUENCE;
        return node;
    }
    return NULL;
}

/**
 * Return an empty sequence value node
 * @return a node
 */
Node *YAMLGetSequenceValueNode() {
    Node *node = getEmptyNode();
    if (node) {
        node->type = SEQUENCE_VALUE;
        return node;
    }
    return NULL;
}

/**
 * Return an empty value node
 * @return a node
 */
Node *YAMLGetValueNode() {
    Node *node = getEmptyNode();
    if (node) {
        node->type = VALUE;
        return node;
    }
    return NULL;
}

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

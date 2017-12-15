//
// Created by Hugo on 05/11/2017.
//

#include <stdlib.h>
#include <string.h>
#include "../../../header/yaml/node.h"
#include "../../../header/yaml/utils/parser.h"
#include "../../../header/yaml/api/data.h"
#include "../../../header/yaml/api/output.h"
#include "../../../header/yaml/api/parse.h"

/**
 * NODE DATA
 */

/**
 * Return an empty map node
 * @param the map key
 * @return a node
 */
Node *YAMLGetMapNode(char *key) {
    if (key) {
        Node *node = getEmptyNode();
        if (node) {
            node->type = MAP;
            YAMLSetKey(node, key);
            return node;
        }
    }
    return NULL;
}

/**
 * Return an empty sequence node
 * @param key sequence key
 * @return a node
 */
Node *YAMLGetSequenceNode(char *key) {
    if (key) {
        Node *node = getEmptyNode();
        if (node) {
            node->type = SEQUENCE;
            YAMLSetKey(node, key);
            return node;
        }
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
 * @param key
 * @param value
 * @return a node
 */
Node *YAMLGetValueNode(char *key, char *value) {
    if (key && value) {
        Node *node = getEmptyNode();
        if (node) {
            node->type = VALUE;
            if (YAMLSetKey(node, key) && YAMLSetValue(node, value)) {
                return node;
            }
        }
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

/**
 * Deep copy a node
 * @param node
 * @return the new node
 */
Node *YAMLDeepCopy(Node *node) {
    char *path = strdup("./resources/tmp.yml");

    if (path) {
        Node *res = NULL;

        if (node && YAMLSaveNode(node, path)) {
            res = YAMLParseFile(path);
            remove(path); // remove tmp path
        }

        free(path);
        return res;
    }

    return NULL;
}

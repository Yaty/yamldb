//
// Created by Hugo on 05/11/2017.
//

#include "../../../header/yaml/node.h"
#include "../../../header/yaml/utils/parser.h"

/**
 * NODE TYPE FUNCTIONS
 */

/**
 * Check if a node has a type
 * @param node
 * @param type
 * @return 1 if true, 0 otherwise
 */
static int nodeIs(Node *node, NodeType type) {
    if (node) {
        return node->type == type;
    }

    return 0;
}

/**
 * Check if a node is a collection (MAP, SEQUENCE, SEQUENCE_VALUE)
 * @param node
 * @return 1 if true, 0 otherwise
 */
int YAMLIsCollection(Node *node) {
    return isCollection(node);
}

/**
 * Check if a Node has the SEQUENCE type
 * @param node
 * @return 1 if true, 0 otherwise
 */
int YAMLIsSequence(Node *node) {
    return nodeIs(node, SEQUENCE);
}

/**
 * Check if a Node has the SEQUENCE_VALUE type
 * @param node
 * @return 1 if true, 0 otherwise
 */
int YAMLIsSequenceValue(Node *node) {
    return nodeIs(node, SEQUENCE_VALUE);
}

/**
 * Check if a Node has the MAP type
 * @param node
 * @return 1 if true, 0 otherwise
 */
int YAMLIsMap(Node *node) {
    return nodeIs(node, MAP);
}

/**
 * Check if a Node has the UNDEFINED type
 * @param node
 * @return 1 if true, 0 otherwise
 */
int YAMLIsUndefined(Node *node) {
    return nodeIs(node, UNDEFINED);
}

/**
 * Check if a Node has the VALUE type
 * @param node
 * @return 1 if true, 0 otherwise
 */
int YAMLIsValue(Node *node) {
    return nodeIs(node, VALUE);
}

/**
 * Get Node type
 * @param node
 * @return a type, UNDEFINED if he don't have one
 */
NodeType YAMLGetType(Node *node) {
    if (node) {
        return node->type;
    }

    return UNDEFINED;
}

/**
 * Set Node type
 * @param node
 * @param type
 * @return 1 for sucess, 0 for failure
 */
int YAMLSetType(Node *node, NodeType type) {
    if (node) {
        node->type = type;
        return 1;
    }

    return 0;
}

//
// Created by Hugo on 05/11/2017.
//

#ifndef CBDD1_TYPE_H
#define CBDD1_TYPE_H

/**
 * Check if a node is a collection (MAP, SEQUENCE, SEQUENCE_VALUE)
 * @param node
 * @return 1 if true, 0 otherwise
 */
int YAMLIsCollection(Node *node);

/**
 * Check if a Node has the SEQUENCE type
 * @param node
 * @return 1 if true, 0 otherwise
 */
int YAMLIsSequence(Node *node);

/**
 * Check if a Node has the SEQUENCE_VALUE type
 * @param node
 * @return 1 if true, 0 otherwise
 */
int YAMLIsSequenceValue(Node *node);

/**
 * Check if a Node has the MAP type
 * @param node
 * @return 1 if true, 0 otherwise
 */
int YAMLIsMap(Node *node);

/**
 * Check if a Node has the UNDEFINED type
 * @param node
 * @return 1 if true, 0 otherwise
 */
int YAMLIsUndefined(Node *node);

/**
 * Check if a Node has the VALUE type
 * @param node
 * @return 1 if true, 0 otherwise
 */
int YAMLIsValue(Node *node);

/**
 * Get Node type
 * @param node
 * @return a type, UNDEFINED if he don't have one
 */
NodeType YAMLGetType(Node *node);

/**
 * Set Node type
 * @param node
 * @param type
 * @return 1 for sucess, 0 for failure
 */
int YAMLSetType(Node *node, NodeType type);

#endif //CBDD1_TYPE_H

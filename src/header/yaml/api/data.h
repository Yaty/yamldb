//
// Created by Hugo on 05/11/2017.
//

#ifndef CBDD1_DATA_H
#define CBDD1_DATA_H

/**
 * Return an empty map node
 * @return a node
 */
Node *YAMLGetMapNode();

/**
 * Return an empty sequence node
 * @return a node
 */
Node *YAMLGetSequenceNode();

/**
 * Return an empty sequence value node
 * @return a node
 */
Node *YAMLGetSequenceValueNode();

/**
 * Return an empty value node
 * @return a node
 */
Node *YAMLGetValueNode();

/**
 * Get Node Value
 * @param node
 * @return the value if she exists, NULL otherwise
 */
char *YAMLGetValue(Node *node);

/**
 * Set Node value
 * @param node
 * @param value
 * @return 1 for success, 0 otherwise
 */
int YAMLSetValue(Node *node, char *value);

/**
 * Get Node key
 * @param node
 * @return a key if she exists, NULL otherwise
 */
char *YAMLGetKey(Node *node);

/**
 * Set Node key
 * @param node
 * @param key
 * @return 1 for success, 0 otherwise
 */
int YAMLSetKey(Node *node, char *key);

#endif //CBDD1_DATA_H

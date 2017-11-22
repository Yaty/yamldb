//
// Created by Hugo on 05/11/2017.
//

#ifndef CBDD1_CHILDREN_H
#define CBDD1_CHILDREN_H

/**
 * Get the number of child in a Node
 * @param node
 * @return the number of child
 */
int YAMLGetSize(Node *node);

/**
 * Get child from a Node at index
 * @param node
 * @param index
 * @return the child, or NULL if he doesn't exists
 */
Node *YAMLGetChildAtIndex(Node *node, int index);

/**
 * Get child by it's key
 * @param node
 * @param key
 * @return the child, or NULL if not found
 */
Node *YAMLGetChildByKey(Node *node, char *key);

/**
 * Get child by it's value
 * @param node
 * @param value
 * @return the child, or NULL if not found
 */
Node *YAMLGetChildByValue(Node *node, char *value);

/**
 * Add a child to a Node
 * @param parent
 * @param child
 * @return 1 for success, 0 otherwise
 */
int YAMLAddChild(Node *parent, Node *child);

/**
 * Add value child to parent from strings
 * @param parent
 * @param key
 * @param value
 * @return 1 for success, 0 for failure
 */
int YAMLAddValueChild(Node *parent, char *key, char *value);

/**
 * Remove a child from a Node at index
 * It will move all other children and resize the array
 * @param parent
 * @param index
 * @return 1 for success, 0 otherwise
 */
int YAMLRemoveChildAtIndex(Node *parent, int index);

/**
 * Add a child to a parent
 * @param parent
 * @param child
 * @param index
 * @return 1 if the child was added, 0 otherwise
 */
int YAMLAddChildAtIndex(Node *parent, Node *child, int index);

/**
 * Remove all children from a Node
 * @param parent the Node
 * @return 1 if all children were removed with success, 0 if an error happened
 */
int YAMLRemoveChildren(Node *parent);

/**
 * Set Node children
 * @param parent
 * @param children
 * @param childrenNumber
 * @return 1 for success, 0 otherwise
 */
int YAMLSetChildren(Node *parent, Node *children, int childrenNumber);

#endif //CBDD1_CHILDREN_H

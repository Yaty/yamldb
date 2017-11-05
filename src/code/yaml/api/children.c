//
// Created by Hugo on 05/11/2017.
//

#include <stdlib.h>
#include "../../../header/yaml/node.h"
#include "../../../header/yaml/utils/parser.h"
#include "../../../header/yaml/api/free.h"
#include "../../../header/yaml/api/children.h"
#include "../../../header/yaml/api/type.h"

/**
 * NODE CHILDREN FUNCTIONS
 */

/**
 * Get the number of child in a Node
 * @param node
 * @return the number of child
 */
int YAMLGetSize(Node *node) {
    if (node && node->childrenNumber) {
        return node->childrenNumber;
    }

    return 0;
}

/**
 * Get Node children
 * @param node
 * @return
 */
Node *YAMLGetChildren(Node *node) {
    if (node && node->children) {
        return node->children;
    }

    return NULL;
}

/**
 * Get child from a Node at index
 * @param node
 * @param index
 * @return the child, or NULL if he doesn't exists
 */
Node *YAMLGetChildAtIndex(Node *node, int index) {
    if (node && index >= 0) {
        if (node->childrenNumber > index) {
            return &(node->children[index]);
        }
    }

    return NULL;
}

/**
 * Add a child to a Node
 * @param parent
 * @param child
 * @return 1 for success, 0 otherwise
 */
int YAMLAddChild(Node *parent, Node *child) {
    if (parent && child) {
        return addChild(parent, child);
    }

    return 0;
}

/**
 * Remove a child from a Node at index
 * It will move all other children and resize the array
 * @param parent
 * @param index
 * @return 1 for success, 0 otherwise
 */
int YAMLRemoveChildAtIndex(Node *parent, int index) {
    Node *child = YAMLGetChildAtIndex(parent, index);

    if (child) {
        int i;
        int success = YAMLFreeNode(child);

        for (i = index; i < YAMLGetSize(parent) - 1; i++) {
            parent->children[i] = parent->children[i + 1];
        }

        parent->childrenNumber -= 1;
        parent->children = (Node*) realloc(parent->children, sizeof(Node) * parent->childrenNumber);

        if (parent->children) {
            return success;
        }
    }

    return 0;
}

/**
 * Add a child to a parent
 * @param parent
 * @param child
 * @param index
 * @return 1 if the child was added, 0 otherwise
 */
int YAMLAddChildAtIndex(Node *parent, Node *child, int index) {
    if (parent && child && YAMLIsCollection(parent)) {
        int childrenNumber = YAMLGetSize(parent);
        if (index <= childrenNumber) {
            int i;

            parent->children = (Node*) realloc(parent->children, sizeof(Node) * (parent->childrenNumber + 1));

            if (parent->children) {
                for (i = childrenNumber; i > index; i++) {
                    parent->children[i] =  parent->children[i - 1];
                }

                parent->childrenNumber += 1;
                parent->children[index] = *child;

                return 1;
            }
        }
    }

    return 0;
}

/**
 * Remove all children from a Node
 * @param parent the Node
 * @return 1 if all children were removed with success, 0 if an error happened
 */
int YAMLRemoveChildren(Node *parent) {
    if (parent) {
        int i;
        int success = 1;

        for (i = 0; i < YAMLGetSize(parent); i++) {
            if(!YAMLFreeNode(&(parent->children[i]))) {
                success = 0;
            }
        }

        parent->childrenNumber = 0;
        parent->children = (Node*) realloc(parent->children, 0);

        if (parent->children) {
            return success;
        }
    }

    return 0;
}

/**
 * Set Node children
 * @param parent
 * @param children
 * @param childrenNumber
 * @return 1 for success, 0 otherwise
 */
int YAMLSetChildren(Node *parent, Node *children, int childrenNumber) {
    if (parent && children) {
        int i;
        int success = 1;

        for (i = 0; i < YAMLGetSize(parent); i++) {
            if(!YAMLFreeNode(&(parent->children[i]))) {
                success = 0;
            }
        }

        parent->children = (Node*) realloc(parent->children, sizeof(Node) * childrenNumber);

        if (parent->children) {
            parent->children = children;
            return success;
        }
    }

    return 0;
}
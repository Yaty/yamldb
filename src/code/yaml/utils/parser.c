//
// Created by Hugo on 28/10/2017.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../../header/string_array_functions.h"
#include "../../../header/yaml/utils/parser.h"

#define BUFFER_SIZE 512

#ifdef _WIN32
#define MODIFIERS "%[^:]: %[^]"
#else
#define MODIFIERS "%[^:]: %s"
#endif

int nodes = 0;

/**
 * Check if a node is a collection
 * @param node
 * @return 1 if true, 0 if false
 */
int isCollection (Node *node) {
    if (node) {
        return node->type == SEQUENCE || node->type == SEQUENCE_VALUE || node->type == MAP;
    }
    return 0;
}

/**
 * Add a child to a parent node
 * @param parent
 * @param child
 * @return 1 if added, 0 if not
 */
int addChild (Node *parent, Node *child) {
    if (parent && child) {
        if (isCollection(parent)) {
            Node *newChildren = (Node*) realloc(parent->children, sizeof(Node) * (parent->childrenNumber + 1));
            if (newChildren) {
                parent->childrenNumber += 1;
                parent->children = newChildren;
                parent->children[parent->childrenNumber - 1] = *child;
                return 1;
            }
        }
    }

    return 0;
}

/**
 * Make an empty node
 * @return an empty initialized node, NULL if error
 */
Node *getEmptyNode() {
    Node *node = (Node*) malloc(sizeof(Node));
    if (node) {
        node->childrenNumber = 0;
        node->key = (char*) malloc(sizeof(char) * BUFFER_SIZE);
        node->value = (char*) malloc(sizeof(char) * BUFFER_SIZE);
        node->children = (Node*) malloc(0);
        node->type = UNDEFINED;
        node->id = nodes++;
        return node;
    }
    return NULL;
}

/**
 * Get key value from a file line, the line if trimmed
 * @param str the line
 * @param key
 * @param value
 * @return 1 for success, 0 for failure
 */
int getKeyValueFromStringSanitized (char *str, char *key, char *value) {
    if (str && key && value) {
        if (getKeyValueFromString(str, key, value)) {
            strcpy(key, trim(key));
            strcpy(value, trim(value));
            return 1;
        }
    }

    return 0;
}

/**
 * Check if a string starts with "- ", contains an alphanumeric key which is followed by a ':'
 * Example : "     - bla:..." -> true, "   d  - bla:..." -> false
 * @param sequence a string
 * @return 1 is it's valid, 0 otherwise
 */
int isValidSequenceInitializer (char *sequence) {
    if (sequence) {
        char *trimmedSequence = trim(sequence);
        size_t length = strlen(trimmedSequence);
        if (length >= 4 && trimmedSequence[0] == '-' && trimmedSequence[1] == ' ') { // "- a:" -> at least 4
            int colonIndex = getCharIndex(trimmedSequence, ':');
            if (colonIndex >= 3) {
                int keyLength = colonIndex - 2; // - 2 to not count "- "
                char key[keyLength + 1]; // + 1 for \0
                substring(trimmedSequence, key, 2, keyLength); // Substring to retrieve the key without "- " and ':'
                if (strlen(key) > 0 && isAlphanumeric(key, 1)) {
                    return 1;
                }
            }
        }
    }

    return 0;
}

int isValidMapInitializer (char *str) {
    if (str) {
        char *trimmedStr = trim(str);
        char key[BUFFER_SIZE];
        char value[BUFFER_SIZE];
        if (getKeyValueFromStringSanitized(trimmedStr, key, value) && isAlphanumeric(key, 1) && strlen(value) >= 0) {
            return 1;
        }
    }

    return 0;
}

/**
 * Retrieve a key and a value from a string
 * Example : "    aaa: bbb    " -> key = "aaa" and value = "bbb"
 * @param str the input string
 * @param key a string where the key will be put
 * @param value a string where the value will be put
 * @return 1 for success, 0 for failure
 */
int getKeyValueFromString (char *str, char *key, char *value) {
    if (str && key && value) {
        return sscanf(str, MODIFIERS, key, value);
    }
}

/**
 * This function retrieve values from a collection
 *
 * If the parent is a SEQUENCE_VALUE
 * data: -> SEQUENCE
 *   - id: 1                        } VALUE } THIS
 *     name: Michel -> data[0][1]   } VALUE } IS
 *     lastname: Dupont             } VALUE } A SEQUENCE
 *     address: 8 rue de l'église   } VALUE } VALUE
 * If it's a map it will get all map pair of key/values.
 * @return 1 for success, 0 for failure
 */
int retrieveCollectionValues (Node *parent, FILE *file) {
    if (parent && isCollection(parent) && file) {
        char buffer[BUFFER_SIZE];
        int firstIteration = 1;
        int childrenPrefixSpaces = 0;
        int currentPrefixSpaces = 0;

        while (fgets(buffer, BUFFER_SIZE, file)) {
            // Init
            if (firstIteration) {
                childrenPrefixSpaces = countPrefixSpaces(buffer);
                if (parent->type == SEQUENCE_VALUE) {
                    childrenPrefixSpaces += 2; // count "- "
                    buffer[childrenPrefixSpaces - 2] = ' '; // remove '-'
                }
                firstIteration = 0;
            }

            // Breaking condition
            currentPrefixSpaces = countPrefixSpaces(buffer);
            if (currentPrefixSpaces != childrenPrefixSpaces) { // We are no longer in the sequence
                fseek(file, -strlen(buffer), SEEK_CUR); // move back
                break;
            }

            if (!parseLine(parent, buffer, file)) {
                return 0;
            }
        }

        return 1;
    }

    return 0;
}

/**
 * This function retrieve a collection
 * If it's a sequence :
 * data: -> a sequence
 *   - id: 1                        }   THIS
 *     name: Michel -> data[0][1]   }   IS A
 *     lastname: Dupont             }   SEQUENCE
 *     address: 8 rue de l'église   }   VALUE
 *   - id: 2                        }   THIS
 *     name: Michel -> data[1][1]   }   IS ANOTHER
 *     lastname: Dupont             }   SEQUENCE
 *     address: 8 rue de l'église   }   VALUE
 * If it's a map:
 * data:
 *   id: 1
 *   name: Michel -> data[0][1]
 *   lastname: Dupont
 *   address: 8 rue de l'église
 * @return 1 for success, 0 for failure
 */
int retrieveCollection (Node *parent, FILE *file) {
    if (parent && file) {
        char buffer[BUFFER_SIZE];
        Node *child;

        while (fgets(buffer, BUFFER_SIZE, file)) { // One loop = one sequence value
            fseek(file, -strlen(buffer), SEEK_CUR); // move back to the key
            if (isValidSequenceInitializer(buffer)) {
                parent->type = SEQUENCE;
                child = getEmptyNode();
                if (child) {
                    child->type = SEQUENCE_VALUE;
                    if (!retrieveCollectionValues(child, file)) {
                        return 0;
                    }
                    if (!addChild(parent, child)) {
                        return 0;
                    }
                }
            } else if (isValidMapInitializer(buffer) && parent->type != SEQUENCE) {
                parent->type = MAP;
                if (!retrieveCollectionValues(parent, file)) {
                    return 0;
                }
                break; // a map has only one set of values unlike a sequence
            } else {
                break;
            }
        }

        return 1;
    }

    return 0;
}

/**
 * Parse a line of a YAML file
 * @param parent the node parent
 * @param line
 * @param file
 * @return 1 for success, 0 for failure
 */
int parseLine (Node *parent, char *line, FILE *file) {
    if (parent && line && file) {
        char key[BUFFER_SIZE];
        char value[BUFFER_SIZE];

        // Reset
        key[0] = '\0';
        value[0] = '\0';

        if (getKeyValueFromStringSanitized(line, key, value) && isAlphanumeric(key, 1)) {
            Node *currentNode = getEmptyNode();
            if (currentNode) {
                if (strlen(value) == 0) { // Collection
                    currentNode->key = strdup(key);
                    if (!retrieveCollection(currentNode, file)) {
                        return 0;
                    }
                } else { // Value
                    currentNode->type = VALUE;
                    currentNode->key = strdup(key);
                    currentNode->value = strdup(value);
                }

                return addChild(parent, currentNode);
            }
        }
    }

    return 0;
}

/**
 * Parse a YAML file
 * @param file stream
 * @return a filled Node
 */
Node *parseFile (FILE *file) {
    if (file) {
        Node *root = getEmptyNode();
        if (root) {
            char line[BUFFER_SIZE];

            root->type = MAP;
            root->key = strdup("root");

            while(fgets(line, BUFFER_SIZE, file)) {
               if (!parseLine(root, line, file)) {
                   break; // error
               }
            }

            return root;
        }
    }

    return NULL;
}
//
// Created by Hugo on 28/10/2017.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../header/string_array_functions.h"
#include "../../header/yaml/parser.h"

#define BUFFER_SIZE 512

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
 */
void addChild (Node *parent, Node *child) {
    if (parent && child) {
        if (isCollection(parent)) {
            parent->childrenNumber += 1;
            parent->children = (Node*) realloc(parent->children, sizeof(Node) * parent->childrenNumber);
            parent->children[parent->childrenNumber - 1] = *child;
            child->parentId = parent->id;
        } else {
            // printf("Can't add a child to a non collection typed node.\n");
        }
    } else {
        // printf("Warn : addChild, parent or child is null.\n");
    }
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
        node->type = UNDEFINED;
        node->id = nodes++;
        return node;
    }
    return NULL;
}

void getKeyValueFromStringSanitized (char *str, char *key, char *value) {
    if (str && key && value) {
        getKeyValueFromString(str, key, value);
        strcpy(key, trim(key));
        strcpy(value, trim(value));
    }
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
            size_t colonIndex = getCharIndex(trimmedSequence, ':');
            if (colonIndex >= 3) {
                size_t keyLength = colonIndex - 2; // - 2 to not count "- "
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
        getKeyValueFromStringSanitized(trimmedStr, key, value);
        if (isAlphanumeric(key, 1) && strlen(value) >= 0) {
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
 */
void getKeyValueFromString (char *str, char *key, char *value) {
    if (str && key && value) {
        sscanf(str, "%[^:]: %[^]", key, value); // [^:] everything except ':' and [^] is everything
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
 */
void retrieveCollectionValues (Node *parent, FILE *file) {
    if (parent && isCollection(parent) && file) {
        char buffer[BUFFER_SIZE];
        int firstIteration = 1;
        int childsPrefixSpaces = 0;
        int currentPrefixSpaces = 0;

        while (fgets(buffer, BUFFER_SIZE, file)) {
            // Init
            if (firstIteration) {
                childsPrefixSpaces = countPrefixSpaces(buffer);
                if (parent->type == SEQUENCE_VALUE) {
                    childsPrefixSpaces += 2; // count "- "
                    buffer[childsPrefixSpaces - 2] = ' '; // remove '-'
                }
                firstIteration = 0;
            }

            // Breaking condition
            currentPrefixSpaces = countPrefixSpaces(buffer);
            if (currentPrefixSpaces != childsPrefixSpaces) { // We are no longer in the sequence
                fseek(file, -strlen(buffer), SEEK_CUR); // move back
                break;
            }

            parseLine(parent, buffer, file);
        }
    }
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
 */
void retrieveCollection (Node *parent, FILE *file) {
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
                    retrieveCollectionValues(child, file);
                    addChild(parent, child);
                }
            } else if (isValidMapInitializer(buffer) && parent->type != SEQUENCE) {
                parent->type = MAP;
                retrieveCollectionValues(parent, file);
                break; // a map has only one set of values unlike a sequence
            } else {
                break;
            }
        }
    }
}

/**
 * Parse a line of a YAML file
 * @param parent the node parent
 * @param line
 * @param file
 * @return a filled Node struct
 */
void parseLine (Node *parent, char *line, FILE *file) {
    if (parent && line && file) {
        char key[BUFFER_SIZE];
        char value[BUFFER_SIZE];
        Node *currentNode;

        // Reset
        key[0] = '\0';
        value[0] = '\0';

        // Get a pair of sanitized key/value
        getKeyValueFromStringSanitized(line, key, value);
        if (isAlphanumeric(key, 1)) {
            currentNode = getEmptyNode();
            if (currentNode) {
                if (strlen(value) == 0) { // Collection
                    currentNode->key = strdup(key);
                    retrieveCollection(currentNode, file);
                } else { // Value
                    currentNode->type = VALUE;
                    currentNode->key = strdup(key);
                    currentNode->value = strdup(value);
                }
                addChild(parent, currentNode);
            }
        }
    }
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
               parseLine(root, line, file);
            }

            return root;
        }
    }

    return NULL;
}
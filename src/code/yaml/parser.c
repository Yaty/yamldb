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
 * Add a child to a parent node
 * @param parent
 * @param child
 */
void parserAddChild (Node *parent, Node *child) {
    if (parent && child) {
        if (parent->type == SEQUENCE || parent->type == SEQUENCE_VALUE) {
            parent->childrenNumber += 1;
            parent->children = realloc(parent->children, sizeof(Node) * parent->childrenNumber);
            parent->children[parent->childrenNumber - 1] = *child;
            printf("%d\n", parent->id);
            child->parentId = parent->id;
        } else {
            printf("Can't add a child to a non sequence typed node.\n");
        }
    } else {
        printf("Warn : addChild, parent or child is null.\n");
    }
}

/**
 * Make an empty node
 * @return an empty initialized node, NULL if error
 */
Node* parserGetEmptyNode() {
    Node* node = (Node*) malloc(sizeof(Node));
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

/**
 * Check if a string contains only a-z A-Z 0-9 _ -
 * @param key
 * @return
 */
int parserIsValidYamlKey (char *key) {
    if (key) {
        size_t keyLength = strlen(key);
        int isValid = 1;

        if (keyLength == 0) {
            return 0;
        }

        for (int i = 0; i < keyLength; i++) {
            char car = key[i];
            if ((car >= 'a' && car <= 'z')
                || (car >= 'A' && car <= 'Z')
                || (car >= '0' && car <= '9')
                || car == '-' || car == '_') {
                continue;
            } else {
                return 0;
            }
        }

        return isValid;
    }

    return 0;
}

/**
 * Set a key and a value to a node
 * The node must be a VALUE NodeType
 * @param node
 * @param key
 * @param value
 */
void parserSetNodeKeyValue (Node *node, char *key, char *value) {
    if (node && key && value) {
        if (node->type == VALUE) {
            node->key = strdup(key);
            node->value = strdup(value);
        } else {
            printf("Can't set node key value to a non Value type node.\n");
        }
    } else {
        printf("Invalid node, key or value : setNodeKeyValue.\n");
    }
}

/**
 * Check if a string starts with "- ", it ignores the spaces before
 * Example : "     - bla..." -> true, "   d  - bla..." -> false
 * @param sequence
 * @return 1 is it's valid, 0 otherwise
 */
int parserIsValidSequenceInitializer (char *sequence) {
    if (sequence) {
        char* trimmedSequence = trim(sequence);
        if (strlen(trimmedSequence) > 1) {
            return trimmedSequence[0] == '-' && trimmedSequence[1] == ' ';
        } else {
            printf("Wrong sequence initializer %s\n", sequence);
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
void parserGetKeyValueFromString (char *str, char *key, char *value) {
    if (str && key && value) {
        sscanf(str, "%[^:]: %[^]", key, value);
    }
}

/**
 * This function retrieve values from a sequence value
 * data:
 *   - id: 1                        }   SEQUENCE VALUE CHILD
 *     name: Michel -> data[0][1]   }   SEQUENCE VALUE CHILD
 *     lastname: Dupont             }   SEQUENCE VALUE CHILD
 *     address: 8 rue de l'église   }   SEQUENCE VALUE CHILD
 */
void *parserRetrieveSequenceValueChilds (Node *parent, FILE *file) {
    if (parent && file) {
        char buffer[BUFFER_SIZE];
        int firstIteration = 1;
        int childsPrefixSpaces = 0;
        int currentPrefixSpaces = 0;

        while (fgets(buffer, BUFFER_SIZE, file)) {
            // Init
            if (firstIteration) {
                childsPrefixSpaces = countPrefixSpaces(buffer) + 2; // count "- "
                buffer[childsPrefixSpaces - 2] = ' ';
                firstIteration = 0;
            }

            // Breaking condition
            currentPrefixSpaces = countPrefixSpaces(buffer);
            if (currentPrefixSpaces != childsPrefixSpaces) { // We are no longer in the sequence
                fseek(file, -strlen(buffer), SEEK_CUR); // move back
                break;
            }

            parserParseLine(parent, buffer, file);
        }
    }
}

/**
 * This function retrieve sequence into an array of Node
 * data:
 *   - id: 1                        }   THIS
 *     name: Michel -> data[0][1]   }   IS A
 *     lastname: Dupont             }   SEQUENCE
 *     address: 8 rue de l'église   }   VALUE
 */
void *parserRetrieveSequence (Node* parent, FILE *file) {
    if (parent && file) {
        char buffer[BUFFER_SIZE];
        Node* sequenceValue;

        while (fgets(buffer, BUFFER_SIZE, file)) { // One loop = one sequence value
            fseek(file, -strlen(buffer), SEEK_CUR); // move back to the key
            if (parserIsValidSequenceInitializer(buffer)) {
                sequenceValue = parserGetEmptyNode();
                if (sequenceValue) {
                    sequenceValue->type = SEQUENCE_VALUE;
                    parserRetrieveSequenceValueChilds(sequenceValue, file);
                    parserAddChild(parent, sequenceValue);
                }
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
void *parserParseLine (Node *parent, char *line, FILE *file) {
    if (parent && line && file) {
        char key[BUFFER_SIZE];
        char value[BUFFER_SIZE];
        Node *currentNode;

        // Reset
        key[0] = '\0';
        value[0] = '\0';

        // Get a pair of key/value and sanitize them with trim
        parserGetKeyValueFromString(line, key, value);
        strcpy(key, trim(key));

        if (parserIsValidYamlKey(key)) {
            currentNode = parserGetEmptyNode();
            if (currentNode) {
                strcpy(value, trim(value));
                if (strlen(value) == 0) { // Sequence
                    currentNode->key = strdup(key);
                    currentNode->type = SEQUENCE;
                    parserRetrieveSequence(currentNode, file);
                } else { // Value
                    currentNode->type = VALUE;
                    parserSetNodeKeyValue(currentNode, key, value);
                }
                parserAddChild(parent, currentNode);
            }
        }
    }
}

/**
 * Parse a YAML file
 * @param file stream
 * @return a filled Node
 */
Node *parserParseFile (FILE *file) {
    if (file) {
        Node *root = parserGetEmptyNode();
        if (root) {
            char line[BUFFER_SIZE];

            root->type = SEQUENCE;
            root->key = "root";

            while(fgets(line, BUFFER_SIZE, file)) {
                parserParseLine(root, line, file);
            }

            return root;
        }
    }

    return NULL;
}
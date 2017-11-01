//
// Created by Hugo on 28/10/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 512

typedef enum NodeType { VALUE, SEQUENCE, SEQUENCE_VALUE } NodeType;
typedef struct Node Node;

/**
 * This structure represent a YAML tree of Node
 * Each node is a pair of key/value
 * A value can be a string, or a sequence
 * If a value is a string then 'char *value' is filled.
 * If it's a sequence 'Node *children' is filled
 *
 * Example :
 * data: -> data is a Node of type SEQUENCE
 *   - id: 1 -> data[0] is a Node of type SEQUENCE_VALUE, data[0][0] is a VALUE Node
 *     name: Michel -> data[0][1] -> VALUE Node
 *     lastname: Dupont -> idem
 *     address: 8 rue de l'église -> idem
 *   - id: 2 -> data[1] -> SEQUENCE_VALUE Node, data[1][0] is a VALUE Node
 *     name: Dupont -> data[1][1] -> VALUE Node
 *     lastname: Michel -> idem
 *     address: 5 rue de l'église -> idem
 */
struct Node {
    NodeType type;
    char *key;
    char *value;
    Node *children;
    size_t childrenNumber;
};

/**
 * Add a child to a parent node
 * @param parent
 * @param child
 */
void addChild (Node *parent, Node *child) {
    if (parent && child) {
        if (parent->type == SEQUENCE || parent->type == SEQUENCE_VALUE) {
            parent->childrenNumber += 1;
            parent->children = realloc(parent->children, sizeof(Node) * parent->childrenNumber);
            parent->children[parent->childrenNumber - 1] = *child;
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
Node* getEmptyNode() {
    Node* node = (Node*) malloc(sizeof(Node));
    if (node) {
        node->childrenNumber = 0;
        node->key = (char*) malloc(sizeof(char) * BUFFER_SIZE);
        node->value = (char*) malloc(sizeof(char) * BUFFER_SIZE);
        return node;
    }
    return NULL;
}

/**
 * Trim a string
 * Example : '    bla bla    ' -> 'bla bla'
 * Remove thoses characters :
 * ' ' (0x20) space (SPC)
 * '\t' (0x09) horizontal tab (TAB)
 * '\n' (0x0a) newline (LF)
 * '\v' (0x0b) vertical tab (VT)
 * '\f' (0x0c) feed (FF)
 * '\r' (0x0d) carriage return (CR)
 * @param str the string to trim
 * @return the trimed string
 */
char *trim(char *str) {
    if (str) {
        size_t strLength = strlen(str);
        char *end;

        // Trim left
        while(isspace((unsigned char)*str)) str++;

        if(*str == 0) { // Empty string
            return str;
        }

        // Trim right
        end = str + strLength - 1;
        while(end > str && isspace((unsigned char)*end)) end--;
        *(end+1) = 0; // \0
        return str;
    }

    printf("Warn : triming an empty string.\n");
    return NULL;
}

/**
 * Check if a string contains only a-z A-Z 0-9 _ -
 * @param key
 * @return
 */
int isValidYamlKey (char *key) {
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

int countPrefixSpaces (char *str) {
    size_t strLength = strlen(str);

    if (str && strLength > 0) {
        int i = 0;
        while (i < strLength && str[i++] == ' ');
        return i - 1; // -1 to remove the last non space character
    }

    printf("Warn : countPrefixSpaces an empty string.\n");
    return 0;
}

void setNodeKeyValue (Node *node, char *key, char *value) {
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

int isValidSequenceInitializer (char *sequence) {
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
 * This function remove X characters at index from a string);
 * @param str
 * @param startPosition, where to start the removal
 * @param length, the number of characters to remove
 */
void removeChars (char *str, int startPosition, int length) {
    if (str && startPosition >= 0 && length > 0) {
        memmove(&str[startPosition], &str[startPosition + length], strlen(str) - startPosition);
    }
}

void getKeyValue (char *str, char *key, char *value) {
    if (str && key && value) {
        sscanf(str, "%[^:]: %s", key, value);
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
Node *retrieveSequenceValueChilds (Node *parent, FILE *file) {
    if (parent && file) {
        char buffer[BUFFER_SIZE];
        char key[BUFFER_SIZE];
        char value[BUFFER_SIZE];
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
                fseek(file, -strlen(buffer), SEEK_CUR); // move back, , + 1 to count '\0'
                break;
            }

            // Reset the key/value pair
            key[0] = '\0';
            value[0] = '\0';

            // Retrieve and sanitize
            getKeyValue(buffer, key, value);
            strcpy(key, trim(key));
            strcpy(value, trim(value));

            // Add child to the parent
            Node* child = getEmptyNode();
            if (child) {
                child->type = VALUE;
                setNodeKeyValue(child, key, value);
                addChild(parent, child);
            }
        }
    }
}

// When we detect that we are no longer in a sequence juste move back the file cursor
/**
 * This function retrieve sequence into an array of Node
 * data:
 *   - id: 1                        }   THIS
 *     name: Michel -> data[0][1]   }   IS A
 *     lastname: Dupont             }   SEQUENCE
 *     address: 8 rue de l'église   }   VALUE
 */
void *retrieveSequence (Node* parent, FILE *file) {
    if (parent && file) {
        char buffer[BUFFER_SIZE];
        Node* sequenceValue;
        parent->type = SEQUENCE;

        while (fgets(buffer, BUFFER_SIZE, file)) { // One loop = one sequence value
            fseek(file, -strlen(buffer), SEEK_CUR); // move back to the key
            if (isValidSequenceInitializer(buffer)) {
                sequenceValue = getEmptyNode();
                if (sequenceValue) {
                    sequenceValue->type = SEQUENCE_VALUE;
                    retrieveSequenceValueChilds(sequenceValue, file);
                    addChild(parent, sequenceValue);
                }
            } else {
                break;
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
            char key[BUFFER_SIZE];
            char value[BUFFER_SIZE];
            Node *currentNode;

            root->type = SEQUENCE;
            root->key = "root";

            while(fgets(line, BUFFER_SIZE, file)) {
                // Reset
                key[0] = '\0';
                value[0] = '\0';

                // Get a pair of key/value and sanitize them with trim
                getKeyValue(line, key, value);
                strcpy(key, trim(key));

                if (isValidYamlKey(key)) {
                    currentNode = getEmptyNode();
                    strcpy(value, trim(value));
                    if (currentNode) {
                        if (strlen(value) == 0) { // Sequence
                            currentNode->key = strdup(key);
                            retrieveSequence(currentNode, file);
                        } else { // Value
                            currentNode->type = VALUE;
                            setNodeKeyValue(currentNode, key, value);
                        }
                        addChild(root, currentNode);
                    }
                }
            }

            return root;
        }
    }

    return NULL;
}

/**
 * Print a Node struct
 * @param node the node to print
 * @param depth initialize with 0 or -1 this value handle indentations
 */
void nodeToString (Node *node, int depth) {
    int i;

    for (i = 0; i < depth; i++) {
        printf("    ");
    }

    if (node->type == VALUE) {
        printf("%s: %s\n", node->key, node->value);
    } else if (node->type == SEQUENCE) {
        printf("%s:\n", node->key);
        for (i = 0; i < node->childrenNumber; i++) {
            nodeToString(&(node->children[i]), depth + 1);
        }
    } else if (node->type == SEQUENCE_VALUE) {
        for (i = 0; i < node->childrenNumber; i++) {
            if (i == 0) {
                printf("- ");
                nodeToString(&(node->children[i]), 0);
            } else {
                printf("  ");
                nodeToString(&(node->children[i]), depth);
            }
        }
    }
}

/**
 * Free recursively a Node and his children
 * @param node the node to free
 */
void freeNode (Node *node) {
    for (int i = 0; i < node->childrenNumber; i++) {
        freeNode(&(node->children[i]));
    }

    free(node->key);
    free(node->value);
    free(node);
}

/**
 * Main test function
 * @param argc
 * @param argv
 * @return
 */
int main (int argc, char **argv) {
    FILE* file = fopen("test.yaml", "r");
    Node* res = parseFile(file);
    nodeToString(res, 0);
    freeNode(res);
    return EXIT_SUCCESS;
}
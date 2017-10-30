//
// Created by Hugo on 28/10/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define BUFFER_SIZE 512

typedef enum NodeType { VALUE, SEQUENCE } NodeType;
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
 *   - id: 1 -> data[0] is a Node of type SEQUENCE, data[0][0] is a VALUE Node
 *     name: Michel -> data[0][1] -> VALUE Node
 *     lastname: Dupont -> idem
 *     address: 8 rue de l'église -> idem
 *   - id: 2 -> data[1] -> SEQUENCE Node, data[1][0] is a VALUE Node
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
    parent->type = SEQUENCE;
    parent->childrenNumber += 1;
    parent->children = realloc(parent->children, sizeof(Node) * parent->childrenNumber);
    parent->children[parent->childrenNumber - 1] = *child;
}

/**
 * Make an empty node
 * @return an empty initialized node
 */
Node* getEmptyNode() {
    Node* node = (Node*) malloc(sizeof(Node));
    node->childrenNumber = 0;
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
    char *end;

    // Trim left
    while(isspace((unsigned char)*str)) str++;

    if(*str == 0) { // Empty string
        return str;
    }

    // Trim right
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    *(end+1) = 0; // \0
    return str;
}

/**
 * Check if a string contains only a-z A-Z 0-9 _ -
 * @param key
 * @return
 */
int isValidYamlKey (char *key) {
    int isValid = 1;
    size_t keyLength = strlen(key);

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

int isValidYamlValue (char *value) {
    return isValidYamlKey(value); // for now a value follow the same rules as a key
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

            while(fgets(line, BUFFER_SIZE, file)) {
                // Reset
                key[0] = '\0';
                value[0] = '\0';

                // Get a pair of key/value and sanitize them with trim
                sscanf(line, "%[^:]: %s", key, value);
                strcpy(key, trim(key));
                strcpy(value, trim(value));

                if (isValidYamlKey(key) && isValidYamlValue(key)) {
                    currentNode = getEmptyNode();
                    currentNode->type = VALUE;
                    if (currentNode) {
                        currentNode->key = strdup(key);
                        currentNode->value = strdup(value);
                        addChild(root, currentNode);
                    }
                } else {
                    // printf("Invalid key or value while parsing : |%s| |%s|\n", key, value);
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
    for (int i = 0; i < depth; i++) {
        printf("\t");
    }

    if (node->type == VALUE) {
        printf("%s: %s\n", node->key, node->value);
    } else if (node->type == SEQUENCE) {
        for (int i = 0; i < node->childrenNumber; i++) {
            nodeToString(&(node->children[i]), depth + 1);
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
    nodeToString(res, -1);
    freeNode(res);
    return EXIT_SUCCESS;
}
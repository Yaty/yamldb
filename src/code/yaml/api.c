//
// Created by Hugo on 01/11/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include "../../header/yaml/node.h"
#include "../../header/yaml/parser.h"
#include "../../header/file_manager.h"

/**
 * Parse a YAML file
 * @param path to the YAML file
 * @return a Node struct representing the YAML
 */
Node *YAMLParseFile(char* path) {
    if (fileExist(path)) {
        FILE* file = fopen(path, "r");
        if (file) {
            Node *parsedFile = parserParseFile(file);
            if (parsedFile) {
                return parsedFile;
            }
        }
    }
    return NULL;
}

/**
 * Print a Node struct
 * @param node the node to print
 * @param depth initialize with 0 or -1 this value handle indentations
 */
void YAMLNodeToString (Node *node, int depth) {
    int i;

    for (i = 0; i < depth; i++) {
        printf("    ");
    }

    // printf("[%d][%d]", node->id, node->parentId);

    if (node->type == VALUE) {
        printf("%s: %s\n", node->key, node->value);
    } else if (node->type == SEQUENCE) {
        printf("%s, %d:\n", node->key, node->childrenNumber);
        for (i = 0; i < node->childrenNumber; i++) {
            YAMLNodeToString(&(node->children[i]), depth + 1);
        }
    } else if (node->type == SEQUENCE_VALUE) {
        for (i = 0; i < node->childrenNumber; i++) {
            if (i == 0) {
                printf("- ");
                YAMLNodeToString(&(node->children[i]), 0);
            } else {
                printf("  ");
                YAMLNodeToString(&(node->children[i]), depth);
            }
        }
    }
}

/**
 * Free recursively a Node and his children
 * @param node the node to free
 */
void YAMLFreeNode (Node *node) {
    for (int i = 0; i < node->childrenNumber; i++) {
        YAMLFreeNode(&(node->children[i]));
    }

    free(node->key);
    free(node->value);
    free(node);
}


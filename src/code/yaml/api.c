//
// Created by Hugo on 01/11/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include "../../header/yaml/parser.h"
#include "../../header/string_array_functions.h"

/**
 * Print in a file arguments if defined
 * Print in console otherwise
 * @param file the file to write to if defined
 * @param argv the number of arguments
 * @param ... the arguments
 */
static void print (FILE *file, int argv, ...) {
    va_list list;
    int i;

    va_start(list, argv);

    if (file) {
        for (i = 0; i < argv; i++) {
            fprintf(file, "%s", va_arg(list, char*));
        }
    } else {
        for (i = 0; i < argv; i++) {
            printf("%s", va_arg(list, char*));
        }
    }

    va_end(list);
}

static void printSpaces (int numbers, FILE *file) {
    int i;
    for (i = 0; i < numbers; i++) {
        print(file, 1, "    ");
    }
}

/**
 * Browse a node and his childs to print them in a YAML way
 * @param node the root node to print
 * @param depth set to 0
 * @param file if specified it will print in that file
 * @param prepend add a string between the spaces and the node
 */
static void output (Node *node, int depth, FILE *file, char *prepend) {
    int i;
    char *pre;

    if (node->type != SEQUENCE_VALUE) {
        printSpaces(depth, file);
    }

    if (prepend && node->type != SEQUENCE_VALUE) {
        print(file, 1, prepend);
    }

    if (node->type == VALUE) {
        print(file, 4, node->key, ": ", node->value, "\n");
    } else if (node->type == SEQUENCE) {
        print(file, 2, node->key, ":\n");
        for (i = 0; i < node->childrenNumber; i++) {
            output(&(node->children[i]), depth + 1, file, prepend);
        }
    } else if (node->type == SEQUENCE_VALUE) {
        for (i = 0; i < node->childrenNumber; i++) {
            pre = concat(prepend, i == 0 ? "- " : "  ");
            output(&(node->children[i]), depth, file, pre);
            free(pre);
        }
    } else if (node->type == MAP) {
        print(file, 2, node->key, ":\n");
        for (i = 0; i < node->childrenNumber; i++) {
            output(&(node->children[i]), depth + 1, file, prepend);
        }
    }
}

/**
 * Parse a YAML file
 * @param path to the YAML file
 * @return a Node struct representing the YAML
 */
Node *YAMLParseFile (char *path) {
    FILE *file = fopen(path, "r");
    if (file) {
        Node *parsedFile = parseFile(file);
        fclose(file);
        return parsedFile;
    }
    return NULL;
}

/**
 * Print a node in the console
 * @param node
 */
void YAMLPrintNode (Node *node) {
    output(node, 0, NULL, NULL);
}

/**
 * Print a node in a file
 * @param node
 * @param path file path, it will overwrite
 * @return 1 for succes, 0 for failure
 */
int YAMLSaveNode (Node *node, char *path) {
    FILE *file = fopen(path, "w+");
    if (file) {
        off_t position;
        int i;

        fflush(file); // https://stackoverflow.com/a/14364557/6456249
        if(strcmp(node->key, "root") == 0) { // remove root node which is added by the parser
            for (i = 0; i < node->childrenNumber; i++) {
                output(&(node->children[i]), 0, file, NULL);
            }
        } else {
            output(node, 0, file, NULL);
        }

        // Remove the last char (\n)
        fseek(file, -1, SEEK_END);
        position = ftell(file);
        ftruncate(fileno(file), position);

        fclose(file);
        return 1;
    }
    return 0;
}

/**
 * Free recursively a Node and his children
 * @param node the node to free
 */
void YAMLFreeNode (Node *node) {
    if (node) {
        int i;
        for (i = 0; i < node->childrenNumber; i++) {
            YAMLFreeNode(&(node->children[i]));
        }

        /*
        if (node->children) {
            free(node->children);
        }
         */

        if (node->key) {
            free(node->key);
        }

        if (node->value) {
            free(node->value);
        }

        free(node);
    }
}


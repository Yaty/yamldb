//
// Created by Hugo on 05/11/2017.
//

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../../../header/yaml/node.h"
#include "../../../header/string_array_functions.h"

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

/**
 * Print 4 * numbers spaces
 * @param numbers
 * @param file
 */
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
    if (node) {
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
                pre = concat(2, prepend, i == 0 ? "- " : "  ");
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
}

/**
 * Save a node into a file
 * @param node
 * @param path
 * @return 1 for succes, 0 otherwise
 */
int YAMLSaveNode(Node *node, char *path) {
    if (node && path) {
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
    }

    return 0;
}

/**
 * Print a node in the console
 * @param node
 */
void YAMLPrintNode (Node *node) {
    if (node) {
        output(node, 0, NULL, NULL);
    }
}

//
// Created by Hugo on 01/11/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "../../header/yaml/node.h"
#include "../../header/yaml/parser.h"
#include "../../header/file_manager.h"

/**
 * Parse a YAML file
 * @param path to the YAML file
 * @return a Node struct representing the YAML
 */
Node *YAMLParseFile (char* path) {
    if (fileExist(path)) {
        FILE* file = fopen(path, "r");
        if (file) {
            Node *parsedFile = parserParseFile(file);
            fclose(file);
            return parsedFile;
        }
    }
    return NULL;
}

/**
 * Print in a file arguments if defined
 * Print in console otherwise
 * @param file the file to write to if defined
 * @param argv the number of arguments
 * @param ... the agumentes
 */
static void print (FILE* file, int argv, ...) {
    va_list list;

    va_start(list, argv);

    if (file) {
        for (int j = 0; j < argv; j++) {
            fprintf(file, "%s", va_arg(list, char*));
        }
    } else {
        for (int j = 0; j < argv; j++) {
            printf("%s", va_arg(list, char*));
        }
    }

    va_end(list);
}

/**
 * Browse a node and his childs to print them in a YAML way
 * @param node the root node to print
 * @param depth set to 0
 * @param file if specified it will print in that file
 */
static void output (Node *node, int depth, FILE* file) {
    int i;

    for (i = 0; i < depth; i++) {
        print(file, 1, "    ");
    }

    if (node->type == VALUE) {
        print(file, 4, node->key, ": ", node->value, "\n");
    } else if (node->type == SEQUENCE) {
        print(file, 2, node->key, ":\n");
        for (i = 0; i < node->childrenNumber; i++) {
            output(&(node->children[i]), depth + 1, file);
        }
    } else if (node->type == SEQUENCE_VALUE) {
        for (i = 0; i < node->childrenNumber; i++) {
            if (i == 0) {
                print(file, 1, "- ");
                output(&(node->children[i]), 0, file);
            } else {
                print(file, 1, "  ");
                output(&(node->children[i]), depth, file);
            }
        }
    }
}

/**
 * Print a node in the console
 * @param node
 */
void YAMLPrintNode (Node *node) {
    output(node, 0, NULL);
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
        fflush(file); // https://stackoverflow.com/a/14364557/6456249
        strcmp(node->key, "root") == 0 // remove root node which is added by the parser
            ? output(&(node->children[0]), 0, file)
            : output(node, 0, file);
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
    for (int i = 0; i < node->childrenNumber; i++) {
        YAMLFreeNode(&(node->children[i]));
    }

    free(node->key);
    free(node->value);
    free(node);
}


//
// Created by Hugo on 09/11/2017.
//

#include <string.h>
#include <stdio.h>
#include "../src/header/yaml/api.h"

int areNodeEquals(Node *n1, Node *n2) {
    int i;

    if (n1 == n2) {
        return 1;
    }

    if (n1 && n2) {
        if (n1->type == n2->type) { // Type
            if (strcmp(n1->key, n2->key) == 0 && strcmp(n1->value, n2->value) == 0) { // Key/value
                if (n1->childrenNumber == n2->childrenNumber) { // Children number
                    for (i = 0; i < n1->childrenNumber; i++) { // Children
                        if (!areNodeEquals(&n1->children[i], &n2->children[i])) {
                            return 0;
                        }
                    }

                    return 1;
                }
            }
        }
    }

    return 0;
}

int filesEquals(char *path1, char *path2) {
    if (path1 && path2) {
        FILE *file1 = fopen(path1, "r");
        FILE *file2 = fopen(path2, "r");
        if (file1 && file2) {
            char char1 = (char) getc(file1);
            char char2 = (char) getc(file2);

            while ((char1 != EOF) && (char2 != EOF) && (char1 == char2)) {
                char1 = (char) getc(file1);
                char2 = (char) getc(file2);
            }

            fclose(file1);
            fclose(file2);

            return char1 == char2;
        }
    }
    return 0;
}

//
// Created by Hugo on 23/11/2017.
//

#include <string.h>
#include <stdlib.h>
#include "../../../header/yaml/api.h"
#include "../../../header/string_array_functions.h"

/**
 * Check if a node is a valid metadata file for storing table informations
 * @param node
 * @return 1 if true, 0 false
 */
int DBIsValidMetadata(Node *node) {
    if (node) {
        int i;
        int size = YAMLGetSize(node);
        Node *child;
        if(YAMLIsMap(node) && size > 0 && areStringsEquals(YAMLGetKey(node), "structure")) {
            for (i = 0; i < size; i++) {
                child = YAMLGetChildAtIndex(node, i);
                if (YAMLIsValue(child) == 0) {
                    return 0;
                }
            }

            return 1;
        }
    }

    return 0;
}

/**
 * Check if a string is a valid column type
 * @param type
 * @return 1 for true, 0 for false
 */
int DBIsValidColumnType(char *type) {
    if (type) {
        char *typeCpy = toLowerCase(strdup(type));
        char *types[] = { "int", "varchar", "char", "float", "double", "text" };
        int typeNumber = sizeof(types) / sizeof(types[0]);
        int i;

        for (i = 0; i < typeNumber; i++) {
            if (areStringsEquals(types[i], type)) {
                free(typeCpy);
                return 1;
            }
        }

        free(typeCpy);
        return 0;
    }

    return 0;
}

/**
 * Check if a table has valid data
 * @param data
 * @return 1 for true, 0 for false
 */
int DBIsValidData(Node *data) {
    // TODO CHECK IS EACH COLUMN IS A VALID ONE (DESCRIBED IN METADATA.YML)
    if (data) {
        int i;
        int j;
        int size2;
        int size1 = YAMLGetSize(data);
        Node *child;
        Node *child2;
        if (YAMLIsSequence(data) && size1 > 0 && areStringsEquals(YAMLGetKey(data), "data")) {
            for (i = 0; i < size1; i++) {
                child = YAMLGetChildAtIndex(data, i);
                size2 = YAMLGetSize(child);
                if (YAMLIsSequenceValue(child) && size2 > 0) {
                    for (j = 0; j < size2; j++) {
                        child2 = YAMLGetChildAtIndex(child, j);
                        if (!YAMLIsValue(child2)) {
                            return 0;
                        }
                    }
                } else {
                    return 0;
                }
            }

            return 1;
        }
    }

    return 0;
}
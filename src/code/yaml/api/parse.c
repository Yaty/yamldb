//
// Created by Hugo on 05/11/2017.
//

#include "../../../header/yaml/node.h"
#include "../../../header/yaml/utils/parser.h"

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

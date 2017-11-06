//
// Created by Hugo on 01/11/2017.
//

#ifndef CBDD1_PARSER_H
#define CBDD1_PARSER_H

#include <stdio.h>
#include "../node.h"

/**
 * Check if a node is a collection
 * @param node
 * @return 1 if true, 0 if false
 */
int isCollection (Node *node);

/**
 * Add a child to a parent node
 * @param parent
 * @param child
 * @return 1 if added, 0 if not
 */
int addChild (Node *parent, Node *child);

/**
 * Make an empty node
 * @return an empty initialized node, NULL if error
 */
Node *getEmptyNode();

/**
 * Check if a string starts with "- ", it ignores the spaces before
 * Example : "     - bla..." -> true, "   d  - bla..." -> false
 * @param sequence
 * @return 1 is it's valid, 0 otherwise
 */
int isValidSequenceInitializer (char *sequence);

/**
 * Retrieve a key and a value from a string
 * Example : "    aaa: bbb    " -> key = "aaa" and value = "bbb"
 * @param str the input string
 * @param key a string where the key will be put
 * @param value a string where the value will be put
 * @return 1 for success, 0 for failure
 */
int getKeyValueFromString (char *str, char *key, char *value);

/**
 * Get key value from a file line, the line if trimmed
 * @param str the line
 * @param key
 * @param value
 * @return 1 for success, 0 for failure
 */
int getKeyValueFromStringSanitized (char *str, char *key, char *value);

/**
 * This function retrieve values from a collection
 *
 * If the parent is a SEQUENCE_VALUE
 * data: -> SEQUENCE
 *   - id: 1                        } VALUE } THIS
 *     name: Michel -> data[0][1]   } VALUE } IS
 *     lastname: Dupont             } VALUE } A SEQUENCE
 *     address: 8 rue de l'église   } VALUE } VALUE
 * If it's a map it will get all map pair of key/values.
 */
void retrieveCollectionValues (Node *parent, FILE *file);

/**
 * This function retrieve a collection
 * If it's a sequence :
 * data: -> a sequence
 *   - id: 1                        }   THIS
 *     name: Michel -> data[0][1]   }   IS A
 *     lastname: Dupont             }   SEQUENCE
 *     address: 8 rue de l'église   }   VALUE
 *   - id: 2                        }   THIS
 *     name: Michel -> data[1][1]   }   IS ANOTHER
 *     lastname: Dupont             }   SEQUENCE
 *     address: 8 rue de l'église   }   VALUE
 * If it's a map:
 * data:
 *   id: 1
 *   name: Michel -> data[0][1]
 *   lastname: Dupont
 *   address: 8 rue de l'église
 */
void retrieveCollection (Node *parent, FILE *file);

/**
 * Parse a line of a YAML file
 * @param parent the node parent
 * @param line
 * @param file
 * @return a filled Node struct
 */
void parseLine (Node *parent, char *line, FILE *file);

/**
 * Parse a YAML file
 * @param file stream
 * @return a filled Node
 */
Node *parseFile (FILE *file);

#endif //CBDD1_PARSER_H

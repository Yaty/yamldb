//
// Created by Hugo on 01/11/2017.
//

#ifndef CBDD1_PARSER_H
#define CBDD1_PARSER_H

#include <stdio.h>
#include "node.h"

/**
 * Add a child to a parent node
 * @param parent
 * @param child
 */
void parserAddChild (Node *parent, Node *child);

/**
 * Make an empty node
 * @return an empty initialized node, NULL if error
 */
Node* parserGetEmptyNode();

/**
 * Check if a string contains only a-z A-Z 0-9 _ -
 * @param key
 * @return
 */
int parserIsValidYamlKey (char *key);

/**
 * Set a key and a value to a node
 * The node must be a VALUE NodeType
 * @param node
 * @param key
 * @param value
 */
void parserSetNodeKeyValue (Node *node, char *key, char *value);

/**
 * Check if a string starts with "- ", it ignores the spaces before
 * Example : "     - bla..." -> true, "   d  - bla..." -> false
 * @param sequence
 * @return 1 is it's valid, 0 otherwise
 */
int parserIsValidSequenceInitializer (char *sequence);

/**
 * Retrieve a key and a value from a string
 * Example : "    aaa: bbb    " -> key = "aaa" and value = "bbb"
 * @param str the input string
 * @param key a string where the key will be put
 * @param value a string where the value will be put
 */
void parserGetKeyValueFromString (char *str, char *key, char *value);

/**
 * This function retrieve values from a sequence value
 * data:
 *   - id: 1                        }   SEQUENCE VALUE CHILD
 *     name: Michel -> data[0][1]   }   SEQUENCE VALUE CHILD
 *     lastname: Dupont             }   SEQUENCE VALUE CHILD
 *     address: 8 rue de l'église   }   SEQUENCE VALUE CHILD
 */
Node *parserRetrieveSequenceValueChilds (Node *parent, FILE *file);

/**
 * This function retrieve sequence into an array of Node
 * data:
 *   - id: 1                        }   THIS
 *     name: Michel -> data[0][1]   }   IS A
 *     lastname: Dupont             }   SEQUENCE
 *     address: 8 rue de l'église   }   VALUE
 */
void *parserRetrieveSequence (Node* parent, FILE *file);

/**
 * Parse a YAML file
 * @param file stream
 * @return a filled Node
 */
Node *parserParseFile (FILE *file);

#endif //CBDD1_PARSER_H

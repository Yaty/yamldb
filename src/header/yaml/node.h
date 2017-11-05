//
// Created by Hugo on 01/11/2017.
//

#ifndef CBDD1_NODE_H
#define CBDD1_NODE_H

typedef enum NodeType { VALUE, SEQUENCE, SEQUENCE_VALUE, MAP, UNDEFINED } NodeType;
typedef struct Node Node;

/**
 * This structure represent a YAML tree of Node
 * Each node is a pair of key/value
 * A value can be a string, or a collection
 * If a value is a string then 'char *value' is filled.
 * If it's a collection 'Node *children' is filled
 *
 * Example with a SEQUENCE :
 * data: -> data is a Node of type SEQUENCE
 *   - id: 1 -> data[0] is a Node of type SEQUENCE_VALUE, data[0][0] is a VALUE Node
 *     name: Michel -> data[0][1] -> VALUE Node
 *     lastname: Dupont -> idem
 *     address: 8 rue de l'église -> idem
 *   - id: 2 -> data[1] -> SEQUENCE_VALUE Node, data[1][0] is a VALUE Node
 *     name: Dupont -> data[1][1] -> VALUE Node
 *     lastname: Michel -> idem
 *     address: 5 rue de l'église -> idem
 * With a map :
 * data: -> MAP
 *     key: bla -> VALUE
 *     lol: lal
 */
struct Node {
    NodeType type;
    char *key;
    char *value;
    Node *children;
    int childrenNumber;
    int id;
};

#endif //CBDD1_NODE_H

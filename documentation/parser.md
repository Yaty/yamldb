# Parser Documentation

## Introduction

Le parser permet de lire un fichier YAML et de le transcrire dans une structure avec laquelle on peut intéragir facilement en C.
Il vient avec un nombre important de fonctions pour manipuler la structure.

## La structure

Un noeud est representé par cette structure :

``` c
struct Node {
    NodeType type;
    char *key;
    char *value;
    Node *children;
    int childrenNumber;
};
```

Description :
- NodeType (enum) : `VALUE`, `SEQUENCE`, `SEQUENCE_VALUE`, `MAP`, `UNDEFINED`
- key : la clé
- value : la valeur
- children : tableau de Node, utilisé par les séquences et les maps qui sont des collections
- childrenNumber : nombre d'enfants du noeud

Un noeud correspond globalement à une paire de clé/valeur dans une fichier YAML.

## Les différents types de noeud

### `VALUE`

Tout simplement une clé et une valeur.

``` yaml
key: value
```

Description :
- NodeType : `VALUE`
- key : "key"
- value : "value"
- children : NULL
- childrenNumber : 0

### `SEQUENCE`

Une liste de liste.

``` yaml
sequence:
    - key: value
      key2: value2
    - key3: value3
      key4: value4
```

Description :
- NodeType : `SEQUENCE`
- key : "sequence"
- value : ""
- children : 2 noeuds de type `SEQUENCE_VALUE`
- childrenNumber : 2

### `SEQUENCE_VALUE`

Les `SEQUENCE_VALUE` sont les éléments d'une séquence, ils contiennent eux même des noeuds de nimporte quel type.

``` yaml
- key: value
  key2: value2
  map:
      key3: value3
```

Description :
- NodeType : `SEQUENCE_VALUE`
- key : ""
- value : ""
- children : 3 noeuds
- childrenNumber : 3

### `MAP`

Un noeud de type `MAP` contient une liste de nimporte quel type de noeud.

``` yaml
map:
    key1: value1
    key2: value2
    map:
        key3: value3
```

Description :
- NodeType : `MAP`
- key : "map"
- value : ""
- children : 3 noeuds
- childrenNumber : 3

### `UNDEFINED`

Valeur par défaut d'un noeud avant d'être rempli.

## Utilisation

### Import

Il faut inclure `api.h`.

``` c
#include "chemin/vers/api.h"
```

### Fonctions

Ci-dessous une liste de toutes les fonctions présente dans l'API.
Le but est d'utiliser le plus possible les fonctions fournit et de ne pas manipuler soit même la structure.
Lire les fichiers sources ou headers pour avoir une description détaillée des fonctions.

#### Parse

Lit un fichier et retourne la structure correspondante.

##### `Node *YAMLParseFile (char *path)`

#### Output

##### `int YAMLSaveNode(Node *node, char *path)`

Transforme une structure au format YAML dans un fichier.

##### `void YAMLPrintNode (Node *node)`

Imprime une structure au format YAML dans la console.

#### Type

##### `int YAMLIsCollection(Node *node)`

Vérifie si un noeud est une collection, càd de type `SEQUENCE`, `SEQUENCE_VALUE` ou `MAP`.

##### `int YAMLIsSequence(Node *node)`

Vérifie si un noeud est de type `SEQUENCE`.

##### `int YAMLIsSequenceValue(Node *node)`

Vérifie si un noeud est de type `SEQUENCE_VALUE`.

##### `int YAMLIsMap(Node *node)`

Vérifie si un noeud est de type `MAP`.

##### `int YAMLIsUndefined(Node *node)`

Vérifie si un noeud est de type `UNDEFINED`.

##### `int YAMLIsValue(Node *node)`

Vérifie si un noeud est de type `VALUE`.

##### `NodeType YAMLGetType(Node *node)`

Récupére le type d'un noeud.

##### `int YAMLSetType(Node *node, NodeType type)`

Fixe le type d'un noeud.

#### Free

##### `int YAMLFreeNode (Node *node)`

Libère la mémoire utilisée par le noeud. Fonction à executer avant de ne plus se servir de ce noeud.

#### Data

##### `Node *YAMLGetMapNode(char *key)`

Récupère un noeud map prêt à être utilisé.

##### `Node *YAMLGetSequenceNode(char *key)`

Récupère un noeud sequence prêt à être utilisé.

##### `Node *YAMLGetSequenceValueNode()`

Récupère un noeud sequence value prêt à être utilisé.

##### `Node *YAMLGetValueNode(char *key, char *value)`

Récupère un noeud value prêt à être utilisé.

##### `char *YAMLGetValue(Node *node)`

Récupère la valeur du noeud.

##### `int YAMLSetValue(Node *node, char *value)`

Fixe la valeur du noeud.

##### `char *YAMLGetKey(Node *node)`

Récupére la valeur de la clé.

##### `int YAMLSetKey(Node *node, char *key)`

Fixe la valeur de la clé.

#### Children

##### `int YAMLGetSize(Node *node)`

Récupère la nombre d'enfants du noeud.

##### `Node *YAMLGetChildAtIndex(Node *node, int index)`

Récupère un noeud enfant d'un noeud à un index.

##### `int YAMLAddChild(Node *parent, Node *child)`

Ajoute un noeud enfant à un noeud parent en dernière position.

##### `int YAMLAddValueChild(Node *parent, char *key, char *value)`

Ajoute un noeud value à un parent avec des strings key/value.

##### `int YAMLRemoveChildAtIndex(Node *parent, int index)`

Retire un enfant d'un noeud à un index.

##### `int YAMLAddChildAtIndex(Node *parent, Node *child, int index)`

Ajoute un noeud enfant à un noeud parent à un index.

##### `int YAMLRemoveChildren(Node *parent)`

Retire et free les enfants d'un noeud parent.

##### `int YAMLSetChildren(Node *parent, Node *children, int childrenNumber)`

Ajoute des noeuds enfants à un noeud parent.

## Todo

### Améliorations

1. Free

### A faire

1.
2.
3;
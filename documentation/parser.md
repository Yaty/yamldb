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
    int id;
};
```

Description :
- NodeType (enum) : VALUE, SEQUENCE, SEQUENCE_VALUE, MAP, UNDEFINED
- key : la clé
- value : la valeur
- children : tableau de Node, utilisé par les séquences et les maps qui sont des collections
- childrenNumber : nombre d'enfants du noeud
- id : un identifiant unique attribué à chaque noeud

Un noeud correspond globalement à une paire de clé/valeur dans une fichier YAML.

## Les différents types de noeud

### VALUE

Tout simplement une clé et une valeur.

``` yaml
key: value
```

Description :
- NodeType : VALUE
- key : "key"
- value : "value"
- children : NULL
- childrenNumber : 0

### SEQUENCE

Une liste de liste.

``` yaml
sequence:
    - key: value
      key2: value2
    - key3: value3
      key4: value4
```

Description :
- NodeType : SEQUENCE
- key : "sequence"
- value : ""
- children : 2 noeuds de type SEQUENCE_VALUE
- childrenNumber : 2

### SEQUENCE_VALUE

Les SEQUENCE_VALUE sont les éléments d'une séquence, ils contiennent eux même des noeuds de nimporte quel type.

``` yaml
- key: value
  key2: value2
  map:
      key3: value3
```

Description :
- NodeType : SEQUENCE_VALUE
- key : ""
- value : ""
- children : 3 noeuds
- childrenNumber : 3

### MAP

Un noeud de type MAP contient une liste de nimporte quel type de noeud.

``` yaml
map:
    key1: value1
    key2: value2
    map:
        key3: value3
```

Description :
- NodeType : MAP
- key : "map"
- value : ""
- children : 3 noeuds
- childrenNumber : 3

### UNDEFINED

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

#### Parse

Lit un fichier et retourne la structure correspondante.
`Node *YAMLParseFile (char *path)`

#### Output

Transforme une structure au format YAML dans un fichier.
`int YAMLSaveNode(Node *node, char *path)`

Imprime une structure au format YAML dans la console.
`void YAMLPrintNode (Node *node)`

#### Type

Vérifie si un noeud est une collection, càd de type `SEQUENCE`, `SEQUENCE_VALUE` ou `MAP`.
`int YAMLIsCollection(Node *node)`

TODO

#### Free

TODO

#### Data

TODO

#### Children

TODO
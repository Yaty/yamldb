//
// Created by Hugo on 30/11/2017.
//

#ifndef CBDD1_HASHMAP_H
#define CBDD1_HASHMAP_H

// Original implementation : https://rosettacode.org/wiki/Associative_arrays/Creation/C

typedef struct HashMap {
    int size;
    int capacity;
    char **keys;
    void **values;
} HashMap;

HashMap *hashNew (int capacity);
void hashInsert (HashMap *h, char *key, void *value);
void *hashLookup (HashMap *h, char *key);

#endif //CBDD1_HASHMAP_H

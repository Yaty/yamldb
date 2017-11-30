//
// Created by Hugo on 30/11/2017.
//

#ifndef CBDD1_HASHMAP_H
#define CBDD1_HASHMAP_H

// https://rosettacode.org/wiki/Associative_arrays/Creation/C

typedef struct HashMap {
    int size;
    void **keys;
    void **values;
} HashMap;

HashMap *hashNew (int size);
int hashIndex (HashMap *h, void *key);
void hashInsert (HashMap *h, void *key, void *value);
void *hashLookup (HashMap *h, void *key);

#endif //CBDD1_HASHMAP_H

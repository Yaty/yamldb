//
// Created by Hugo on 30/11/2017.
//

// https://rosettacode.org/wiki/Associative_arrays/Creation/C

#include <stdio.h>
#include <stdlib.h>
#include "../../header/utils/hashmap.h"

HashMap *hashNew (int size) {
    HashMap *h = calloc(1, sizeof (HashMap));
    h->keys = calloc((size_t) size, sizeof (void *));
    h->values = calloc((size_t) size, sizeof (void *));
    h->size = size;
    return h;
}

int hashIndex (HashMap *h, void *key) {
    int i = (int) key % h->size;
    while (h->keys[i] && h->keys[i] != key){
        i = (i + 1) % h->size;
    }
    return i;
}

void hashInsert (HashMap *h, void *key, void *value) {
    int i = hashIndex(h, key);
    h->keys[i] = key;
    h->values[i] = value;
}

void *hashLookup (HashMap *h, void *key) {
    int i = hashIndex(h, key);
    return h->values[i];
}
//
// Created by Hugo on 30/11/2017.
//

// Original implementation : https://rosettacode.org/wiki/Associative_arrays/Creation/C

#include <stdio.h>
#include <stdlib.h>
#include "../../header/utils/hashmap.h"
#include "../../header/string_array_functions.h"

HashMap *hashNew (int capacity) {
    HashMap *h = calloc(1, sizeof (HashMap));
    h->keys = calloc((size_t) capacity, sizeof (char *));
    h->values = calloc((size_t) capacity, sizeof (void *));
    h->size = 0;
    h->capacity = capacity;
    return h;
}


void hashInsert (HashMap *h, char *key, void *value) {
    if (h->size < h->capacity) {
        int i = h->size;

        for (int j = 0; j < i; j++) {
            if (areStringsEquals(key, h->keys[i], 0)) {
                return; // this name already exist
            }
        }

        h->keys[i] = key;
        h->values[i] = value;
        h->size++;
    }
}

void *hashLookup (HashMap *h, char *key) {
    for (int i = 0; i < h->size; i++) {
        if (areStringsEquals(h->keys[i], key, 0)) {
            return h->values[i];
        }
    }

    return NULL;
}
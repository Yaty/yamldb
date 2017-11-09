//
// Created by Hugo on 09/11/2017.
//

#include "children.c"

int apiTestAll() {
    printf("Testing API ... ");
    char *result = children_all_tests();
    if (result != 0) {
        printf("Error : %s\n", result);
    }
    else {
        printf("OK !\n");
    }

    return result != 0;
}
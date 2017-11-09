//
// Created by Hugo on 04/11/2017.
//

#include "end2end.c"

int parserTestAll() {
    printf("Testing parser ... ");
    char *result = end2end_all_tests();
    if (result != 0) {
        printf("Error : %s\n", result);
    }
    else {
        printf("OK !\n");
    }

    return result != 0;
}
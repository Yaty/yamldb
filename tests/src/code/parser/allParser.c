//
// Created by Hugo on 04/11/2017.
//

#include <stdio.h>

static char *allTests() {
    return 0;
}

int parserTestAll() {
    printf("Testing parser ... ");
    char *result = allTests();
    if (result != NULL) {
        printf("Error : %s\n", result);
    } else {
        printf("OK !\n");
    }

    return result == NULL;
}
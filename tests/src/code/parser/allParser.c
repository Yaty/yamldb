//
// Created by Hugo on 04/11/2017.
//

#include <stdio.h>
#include "../../header/parser/end2end.h"

int parserTestAll() {
    printf("Testing parser ... ");
    char *result = end2endAllTests();
    if (result != NULL) {
        printf("Error : %s\n", result);
    } else {
        printf("OK !\n");
    }

    return result == NULL;
}
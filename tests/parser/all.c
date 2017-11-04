//
// Created by Hugo on 04/11/2017.
//

#include "values.c"

int parserTestAll() {
    char *result = values_all_tests();
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL PARSER TESTS PASSED\n");
    }

    return result != 0;
}
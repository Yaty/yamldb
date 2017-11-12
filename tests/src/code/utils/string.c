//
// Created by Hugo on 12/11/2017.
//

#include <stdio.h>
#include <string.h>
#include "../../header/minunit.h"
#include "../../../../src/header/string_array_functions.h"

static char *trim1() {
    char *msg = strdup("    spacesbefore");
    mu_assert("trim1", strcmp(trim(msg), "spacesbefore") == 0);
    return 0;
}

static char *trim2() {
    char *msg = strdup("spacesafter        ");
    mu_assert("trim2", strcmp(trim(msg), "spacesafter") == 0);
    return 0;
}

static char *trim3() {
    char *msg = strdup("    spaces     ");
    mu_assert("trim3", strcmp(trim(msg), "spaces") == 0);
    return 0;
}

static char *allTests() {
    mu_run_test(trim1);
    mu_run_test(trim2);
    mu_run_test(trim3);
    return 0;
}

int stringTestAll() {
    printf("Testing strings functions ... ");
    char *result = allTests();
    if (result != NULL) {
        printf("Error : %s\n", result);
    } else {
        printf("OK !\n");
    }

    return result == NULL;
}
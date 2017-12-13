//
// Created by Hugo on 13/12/2017.
//

//
// Created by Hugo on 12/11/2017.
//

#include <stdio.h>
#include "../../header/minunit.h"
#include "../../../../src/header/utils/type.h"

static char *evalType1() {
    mu_assert("evalType1", evalType("a") == CHAR);
}

static char *evalType2() {
    mu_assert("evalType2", evalType("abc725.abc") == STRING);
}

static char *evalType3() {
    mu_assert("evalType3", evalType("2500") == INT);
}

static char *evalType4() {
    mu_assert("evalType4", evalType("250.369") == DOUBLE);
}

static char *evalType5() {
    mu_assert("evalType5", evalType("") == UNKNOWN);
}

static char *evalType6() {
    mu_assert("evalType6", evalType(NULL) == UNKNOWN);
}

static char *evalType7() {
    mu_assert("evalType7", evalType("256,256") == STRING);
}

static char *allTests() {
    mu_run_test(evalType1);
    mu_run_test(evalType2);
    mu_run_test(evalType3);
    mu_run_test(evalType4);
    mu_run_test(evalType5);
    mu_run_test(evalType6);
    mu_run_test(evalType7);
    return 0;
}

int typeTestAll() {
    printf("Testing type functions ... ");
    char *result = allTests();
    if (result != NULL) {
        printf("Error : %s\n", result);
    } else {
        printf("OK !\n");
    }

    return result == NULL;
}
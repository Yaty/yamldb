//
// Created by Hugo on 10/11/2017.
//

#include "../../header/minunit.h"
#include "../../../../src/header/yaml/api.h"

static char *freeValueNode() {
    // TODO
    return 0;
}

char *freeAllTests() {
    mu_run_test(freeValueNode);
    return 0;
}

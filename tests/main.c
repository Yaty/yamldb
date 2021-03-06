//
// Created by Hugo on 04/11/2017.
//

#include <stdlib.h>
#include <stdio.h>

#include "./src/header/api/allApi.h"
#include "./src/header/parser/allParser.h"
#include "./src/header/utils/string.h"
#include "src/header/utils/type.h"

int main(int argc, char **argv) {
    printf("Launching tests.\n");
    int res = parserTestAll();
    res += apiTestAll();
    res += stringTestAll();
    res += typeTestAll();
    printf(res == 4 ? "Success.\n" : "Failure.\n");
    return EXIT_SUCCESS;
}

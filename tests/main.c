//
// Created by Hugo on 04/11/2017.
//

#include <stdlib.h>
#include <stdio.h>

#include "./src/header/api/allApi.h"
#include "./src/header/parser/allParser.h"

int main(int argc, char **argv) {
    printf("Launching tests.\n");
    int res = parserTestAll();
    res += apiTestAll();
    printf(res == 2 ? "Success.\n" : "Failure.\n");
    return EXIT_SUCCESS;
}

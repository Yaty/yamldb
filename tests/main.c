//
// Created by Hugo on 04/11/2017.
//

#include <stdlib.h>
#include "./parser/all.c"

int main(int argc, char **argv) {
    printf("Launching tests.\n");
    parserTestAll();
    printf("Tests run: %d\n", tests_run);
    printf("TESTS DONE.\n");
    return EXIT_SUCCESS;
}

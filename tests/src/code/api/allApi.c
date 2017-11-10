//
// Created by Hugo on 09/11/2017.
//

#include <stdio.h>
#include "../../header/api/children.h"
#include "../../header/api/data.h"

int apiTestAll() {
    printf("Testing API ... ");
    char *children = childrenAllTests();
    char *data = dataAllTests();

    if (children != NULL) {
        printf("Error children : %s\n", children);
    } else if (data != NULL) {
        printf("Error data : %s\n", data);
    } else {
        printf("OK !\n");
    }

    return children == NULL && data == NULL;
}
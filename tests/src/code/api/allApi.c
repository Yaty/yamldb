//
// Created by Hugo on 09/11/2017.
//

#include <stdio.h>
#include "../../header/api/children.h"
#include "../../header/api/data.h"
#include "../../header/api/free.h"
#include "../../header/api/type.h"

int apiTestAll() {
    printf("Testing YAML API ... ");
    char *children = childrenAllTests();
    char *data = dataAllTests();
    char *free = freeAllTests();
    char *type = typeAllTests();

    if (children != NULL) {
        printf("Error children : %s\n", children);
    } else if (data != NULL) {
        printf("Error data : %s\n", data);
    } else if (free != NULL) {
        printf("Error free : %s\n", free);
    } else if (type != NULL) {
        printf("Error type : %s\n", type);
    } else {
        printf("OK !\n");
    }

    return children == NULL && data == NULL && free == NULL && type == NULL;
}
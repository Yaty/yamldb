#include <stdlib.h>
#include <string.h>
#include "../../header/utils/type.h"

Type evalType(char *str) {
    if (str) {
        char *ptr = "";
        if (strtol(str, &ptr, 10) && strlen(ptr) == 0) {
            return INT;
        } else if (strtod(str, &ptr) && strlen(ptr) == 0) {
            return DOUBLE;
        } else if (strlen(str) == 1) {
            return CHAR;
        } else if (strlen(str) > 1) {
            return STRING;
        }
    }

    return UNKNOWN;
}
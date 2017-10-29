/*
**  Filename : string_array_functions.c
**
**  Made by : Vincent GUÉNIN ESGI - 3AL-1
**
**  Description : Contains string array manipulation related functions
*/

/*================ INCLUDES ================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/general.h"
#include "../header/string_array_functions.h"

/*================ FUNCTIONS ================*/

/*
Goal : Free an entire string array
Input : - length (short), length of the array
        - array (char**), array to free
Output : void
*/
void freeStrArray(short length, char **array) {
    short counter;

    for( counter = 0; counter < length; counter++ ) {
        free(array[counter]);
    }

    free(array);
}

/*
Goal : Malloc an entire string array
Input : - arrayLength (short), length of the array
        - strLength (short), max length of the string into the array
Output : char**, array the function just malloc.
Require : The returned char** must be free after use.
*/
char** mallocStrArray(short arrayLength, short strLength) {
    char **array;
    short counter;

    array = malloc( sizeof(char*) * arrayLength );

    for( counter = 0; counter < arrayLength; counter++ ) {
        array[counter] = malloc( sizeof(char) * strLength );
    }

    return array;
}

/*
Goal : Copy the content of a string array into an other
Input : - dest (char**), array we want to overwrite
        - sourceLength (short), length of the array source
        - source (char**), array we want to copy
Output : void
*/
void copyStrArray(char **dest, short sourceLength, char **source) {
    short counter;

    for( counter = 0; counter < sourceLength; counter++ ) {
        strcpy( dest[counter], source[counter] );
    }
}

/*
Goal : Increase the size of a string array
Input : - currentLength (short), length of the array when it enters the function.
            The variable is modified into the function. At the end, it contains the length
            of the new array.
        - lengthToAdd (short), number of slot we want to add in the array
        - income_array (char***), array to process.
            The array size is modified during process.
Output : char, state of the process :
            - 0, error while memory allocation
            - 1, success
*/
char increaseStrArraySize(short strLength, short *currentLength, short lengthToAdd, char ***income_array) {
    char **temp;
    char **array = *income_array;

    temp = mallocStrArray(*currentLength + lengthToAdd, strLength);
    if( temp == NULL ) {
        return 0;
    }

    copyStrArray(temp, *currentLength, array);

    freeStrArray(*currentLength, array);
    array = temp;
    *currentLength += lengthToAdd;

    *income_array = array;

    return 1;
}

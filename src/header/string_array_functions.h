/*
**  Filename : string_array_functions.c
**
**  Made by : Vincent GUÉNIN ESGI - 3AL-1
**
**  Description : Contains string array manipulation related functions
*/

#ifndef STRING_ARRAY_FUNCTIONS_H_INCLUDED
#define STRING_ARRAY_FUNCTIONS_H_INCLUDED

/*================ STRUCT DECLARATION ================*/


/*================ PROTOTYPES ================*/
/*
Goal : Free an entire string array
Input : - length (short), length of the array
        - array (char**), array to free
Output : void
*/
void freeStrArray(short length, char **array);

/*
Goal : Malloc an entire string array
Input : - arrayLength (short), length of the array
        - strLength (short), max length of the string into the array
Output : char**, array the function just malloc.
Require : The returned char** must be free after use.
*/
char** mallocStrArray(short arrayLength, short strLength);

/*
Goal : Copy the content of a string array into an other
Input : - dest (char**), array we want to overwrite
        - sourceLength (short), length of the array source
        - source (char**), array we want to copy
Output : void
*/
void copyStrArray(char **dest, short sourceLength, char **source);

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
char increaseStrArraySize(short strLength, short *currentLength, short lengthToAdd, char ***income_array);

#endif // STRING_ARRAY_FUNCTIONS_H_INCLUDED

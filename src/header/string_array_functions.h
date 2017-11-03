/*
**  Filename : string_array_functions.c
**
**  Made by : Vincent GU�NIN ESGI - 3AL-1
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

/**
 * This function remove X characters at index from a string);
 * @param str
 * @param startPosition, where to start the removal
 * @param length, the number of characters to remove
 */
void removeChars (char *str, int startPosition, int length);

/**
 * Trim a string
 * Example : '    bla bla    ' -> 'bla bla'
 * Remove thoses characters :
 * ' ' (0x20) space (SPC)
 * '\t' (0x09) horizontal tab (TAB)
 * '\n' (0x0a) newline (LF)
 * '\v' (0x0b) vertical tab (VT)
 * '\f' (0x0c) feed (FF)
 * '\r' (0x0d) carriage return (CR)
 * @param str the string to trim
 * @return the trimed string
 */
char *trim(char *str);

/**
 * Count spaces before another characters
 * Example : "  bla" -> 2
 * @param str the string to analyse
 * @return the number of spaces
 */
int countPrefixSpaces (char *str);

/**
 * Check is a sting is alphanumeric (a-z A-Z 0-9)
 * @param str
 * @param bonus if 1 it will allow _ and -
 * @return 1 if true, 0 if false
 */
int isAlphanumeric (char *str, int bonus);

#endif // STRING_ARRAY_FUNCTIONS_H_INCLUDED

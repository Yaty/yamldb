/*
**  Filename : string_array_functions.c
**
**  Made by : Vincent GU�NIN ESGI - 3AL-1
**
**  Description : Contains string array manipulation related functions
*/

/*================ INCLUDES ================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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

/**
 * This function remove X characters at index from a string);
 * @param str
 * @param startPosition, where to start the removal
 * @param length, the number of characters to remove
 */
void removeChars (char *str, int startPosition, int length) {
    if (str && startPosition >= 0 && length > 0) {
        memmove(&str[startPosition], &str[startPosition + length], strlen(str) - startPosition);
    }
}

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
char *trim(char *str) {
    if (str) {
        size_t strLength = strlen(str);
        char *end;

        // Trim left
        while(isspace((unsigned char)*str)) str++;

        if(*str == 0) { // Empty string
            return str;
        }

        // Trim right
        end = str + strLength - 1;
        while(end > str && isspace((unsigned char)*end)) end--;
        *(end+1) = 0; // \0
        return str;
    }

    // printf("Warn : triming an empty string.\n");
    return NULL;
}

/**
 * Count spaces before another characters
 * Example : "  bla" -> 2
 * @param str the string to analyse
 * @return the number of spaces
 */
int countPrefixSpaces (char *str) {
    size_t strLength = strlen(str);

    if (str && strLength > 0) {
        int i = 0;
        while (i < strLength && str[i++] == ' ');
        return i - 1; // -1 to remove the last non space character
    }

    //printf("Warn : countPrefixSpaces an empty string.\n");
    return 0;
}

/**
 * Check is a sting is alphanumeric (a-z A-Z 0-9)
 * @param str
 * @param bonus if 1 it will allow _ and -
 * @return 1 if true, 0 if false
 */
int isAlphanumeric (char *str, int bonus) {
    if (str) {
        int i;
        size_t strLength = strlen(str);

        if (strLength == 0) {
            return 0;
        }

        for (i = 0; i < strLength; i++) {
            if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= '0' && str[i] <= '9')) {
                continue;
            } else if (bonus && (str[i] == '_' || str[i] == '-')) {
                continue;
            }
            return 0;
        }

        return 1;
    }

    return 0;
}

/**
 * Retrieve char index in a string
 * @param str the string
 * @param c the char we are looking for
 * @return an index >= 0 or NULL if not found
 */
size_t getCharIndex (char *str, char c) {
    if (str && c) {
        char *buffer;

        // Get colon index
        buffer = strchr(str, c);
        if (buffer) {
            return buffer - str; // substractacting the adresss give the char position
        }
    }

    return NULL;
}

/**
 * Substring a string
 * @param str original string
 * @param substr where the substring will be stored
 * @param start index where to start
 * @param length numbers of chars to substring
 */
void substring (char *str, char *substr, int start, size_t length) {
    if (str && start >= 0 && length >= 1) {
        memcpy(substr, &str[start], length);
        substr[length] = '\0';
    }
}

/**
 * Concat two strings into one
 * The generated string needs to be free
 * @param str1 first string
 * @param str2 second string
 */
char *concat (char *str1, char *str2) {
    char *res = malloc(strlen(str1) + strlen(str2));
    strcpy(str1, res);
    strcat(str2, res);
    return res;
}

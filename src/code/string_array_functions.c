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
#include <stdarg.h>
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
 * This function remove X characters at index from a string
 * The string needs to be allocated dynamically
 * @param str
 * @param startPosition, where to start the removal
 * @param length, the number of characters to remove
 */
int removeChars (char *str, int startPosition, int length) {
    if (str && startPosition >= 0 && length > 0) {
        size_t strLength = strlen(str);
        if (length - startPosition <= strLength) {
            memmove(&str[startPosition], &str[startPosition + length], strLength - startPosition);
            return 1;
        }
    }

    return 0;
}

/**
 * Trim a string, the string needs to be allocated dynamically
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
        // Trim left
        while(isspace((unsigned char)*str)) str++;

        if(*str == 0) { // Empty string
            return str;
        }

        // Trim right
        size_t strLength = strlen(str);
        char *end = str + strLength - 1;
        while(end > str && isspace((unsigned char)*end)) end--;
        *(end+1) = '\0';
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
 * @return an index >= 0 or -1 if not found
 */
long getCharIndex (char *str, char c) {
    if (str && c) {
        char *buffer;

        // Get colon index
        buffer = strchr(str, c);
        if (buffer) {
            return buffer - str; // substractacting the adresss give the char position
        }
    }

    return -1;
}

/**
 * Substring a string
 * @param str original string
 * @param substr where the substring will be stored
 * @param start index where to start
 * @param length numbers of chars to substring
 * @return 1 for success, 0 for failure
 */
int substring (char *str, char *substr, int start, size_t length) {
    if (str && substr && start >= 0 && length >= 1) {
        size_t strLength = strlen(str);
        if (start < strLength && length <= strLength) {
            memcpy(substr, &str[start], length);
            substr[length] = '\0';
            return 1;
        }
    }

    return 0;
}

/**
 * Count the total length of all strings
 * @param strings
 * @param a varargs list, this list needs to be started before
 * @return the length
 */
size_t countStringsLength(int strings, va_list list) {
    if (strings > 0 && list) {
        size_t length = 0;
        int i;
        char *tmp;

        for (i = 0; i < strings; i++) {
            tmp = va_arg(list, char *);
            if (tmp) {
                length += strlen(tmp);
            }
        }

        return length;
    }

    return 0;
}

/**
 * Concat strings into one
 * The generated string needs to be free
 * @param numbers of string to concat
 * @param a list of strings
 * @return NULL if error, the concatened string if it succeeded
 */
char *concat (int strings, ...) {
    if (strings > 0) {
        va_list list;
        int i;
        char *res = NULL;
        char *currentStr = NULL;

        va_start(list, strings);
        size_t resLength = countStringsLength(strings, list);

        if (resLength > 0) {
            res = malloc(sizeof(char) * resLength + 1);
            res[0] = '\0';

            for (i = 0; i < strings; i++) {
                currentStr = va_arg(list, char *);
                if (currentStr) {
                    strcat(res, currentStr);
                }
            }

            res[resLength] = '\0';
        }

        va_end(list);
        return res;
    }

    return NULL;
}

/**
 * Uppercase or lowercase a string
 * @param str
 * @param upper
 * @return the string or null if error
 */
static char *transformCase(char *str, int upper) {
    if (str) {
        size_t length = strlen(str);
        int i;
        for (i = 0; i < length; i++) {
            str[i] = upper ? toupper(str[i]) : tolower(str[i]);
        }
        return str;
    }

    return NULL;
}

/**
 * Uppercase a string, the string needs to be allocated dynamically
 * Example : "bla" -> "BLA"
 * @param str
 * @return the uppercase string or null for error
 */
char *toUpperCase(char *str) {
    return transformCase(str, 1);
}

/**
 * Lowercase a string, the string needs to be allocated dynamically
 * Example : "BLA" -> "bla"
 * @param str
 * @return the lowercase string or null for error
 */
char *toLowerCase(char *str) {
    return transformCase(str, 0);
}

/**
 * Check if a string starts with another string
 * @param str
 * @param prefix
 * @param sensitivity 1 for case insensitivity
 * @return 1 for true, 0 for false
 */
int startsWith(char *str, char *prefix, int sensitivity) {
    if (str && prefix) {
        int res;
        char *strCpy = strdup(str);
        char *prefixCpy = strdup(prefix);

        if (sensitivity == 1) {
            strCpy = toLowerCase(strCpy);
            prefixCpy = toLowerCase(prefixCpy);
        }

        size_t prefixLength = strlen(prefixCpy);
        if (prefixLength == 0) return 0;

        res = strncmp(prefixCpy, strCpy, prefixLength);

        free(strCpy);
        free(prefixCpy);

        return res == 0;
    }

    return 0;
}

/**
 * Check if a string ends with another string
 * @param str
 * @param suffix
 * @param sensitivity 1 for case insensitivity
 * @return 1 for true, 0 for false
 */
int endsWith(char *str, char *suffix, int sensitive) {
    if (str && suffix) {
        char *strCpy = strdup(str);
        char *suffixCpy = strdup(suffix);
        int res = 0;
        size_t strLength = strlen(strCpy);
        size_t suffixLength = strlen(suffixCpy);

        if (sensitive == 1) {
            strCpy = toLowerCase(strCpy);
            suffixCpy = toLowerCase(suffixCpy);
        }

        if (suffixLength <= strLength && suffixLength > 0) {
            res = strncmp(strCpy + strLength - suffixLength, suffixCpy, suffixLength) == 0;
        }

        free(strCpy);
        free(suffixCpy);
        return res;
    }

    return 0;
}

/**
 * Get index where a substring is in a string
 * @param str
 * @param substr
 * @param sensitivity 1 for case insensitivity
 * @return an index
 */
long getSubstringIndex(char *str, char *substr, int sensitivity) {
    if (str && substr) {
        long res = -1;
        char *strCpy = strdup(str);
        char *substrCpy = strdup(substr);

        if (sensitivity == 1) {
            strCpy = toLowerCase(strCpy);
            substrCpy = toLowerCase(substrCpy);
        }

        char *buffer = strstr(strCpy, substrCpy);

        if (buffer) {
            res = buffer - strCpy;
        }

        free(strCpy);
        free(substrCpy);

        return res;
    }

    return -1;
}

/**
 * Check if strings are equals
 * @param str1
 * @param str2
 * @param sensitivity 1 for case insensitivity
 * @return 1 for true, 0 for false
 */
int areStringsEquals(char *str1, char *str2, int sensitivity) {
    if (str1 && str2) {
        int res;
        char *str1Cpy = strdup(str1);
        char *str2Cpy = strdup(str2);

        if (sensitivity == 1) {
            str1Cpy = toLowerCase(str1Cpy);
            str2Cpy = toLowerCase(str2Cpy);
        }

        res = strcmp(str1Cpy, str2Cpy);

        free(str1Cpy);
        free(str2Cpy);

        return res == 0;
    }

    return 0;
}

/**
 * Check if a string is inside an array of string
 * @param str
 * @param array
 * @param arraySize
 * @return 1 for true, 0 for false
 */
int stringIntoArray(char *str, char **array, int arraySize) {
    if (str && array && arraySize >= 0) {
        for (int i = 0; i < arraySize; i++) {
            if (areStringsEquals(array[i], str, 0)) {
                return 1;
            }
        }
    }

    return 0;
}

/**
 * Add a string into an array
 * @param string
 * @param array
 * @param arraySize
 * @return 1 for success, 0 for failure
 */
int addStringIntoArray(char *string, char ***array, int arraySize) {
    if (string && array && arraySize >= 0) {
        char **tmp = realloc(*array, (arraySize + 1) * sizeof(char*));
        if (tmp) {
            *array = tmp;
            (*array)[arraySize] = string;
            return 1;
        }
    }

    return 0;
}

/**
 * Remove an element at an index in an array
 * @param array
 * @param arraySize
 * @param index
 * @param freeElement if we free
 */
int removeElementAtIndex(char ***array, int arraySize, int index, int freeElement) {
    if (array && *array && arraySize > 0 && index >= 0) {
        int i;

        if (freeElement) {
            free((*array)[index]);
        }


        for (i = index; i < arraySize - 1; i++) {
            (*array)[index] = (*array)[index + 1];
        }

        (*array)[arraySize - 1] = NULL;
        return 1;
    }

    return 0;
}

/**
 * Make string array deep copy
 * @param columns
 * @param columnsCounter
 * @return copied array
 */
char **makeStringsDeepCopy(char **columns, int columnsCounter) {
    if (columns && columnsCounter > 0) {
        int i;
        char **res = malloc(sizeof(char*) * columnsCounter);

        for (i = 0; i < columnsCounter; i++) {
            res[i] = strdup(columns[i]);
        }

        return res;
    }

    return NULL;
}

/**
 * Remove a number of elements from an index
 * It will move elements to fill the created gaps
 * @param array
 * @param start
 * @param length
 * @return 1 for success, 0 for failure
 */
int splice(void **array, int arrayLength, int start, int length) {
    // Should we realloc array ?
    if (array && start >= 0 && length > 0 && arrayLength > 0 && start + length < arrayLength) {
        int i;
        int j;

        if (arrayLength == 1) {
            array[0] = NULL;
            return 1;
        }

        for (i = 0; i < length; i++) {
            for (j = start; j < arrayLength - 1; j++) {
                array[j] = array[j + 1];
            }
        }

        array[arrayLength - 1] = NULL;

        return 1;
    }

    return 0;
}
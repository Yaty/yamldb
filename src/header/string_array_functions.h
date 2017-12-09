/*
**  Filename : string_array_functions.c
**
**  Made by : Vincent GU�NIN ESGI - 3AL-1
**
**  Description : Contains string array manipulation related functions
*/

#ifndef STRING_ARRAY_FUNCTIONS_H_INCLUDED
#define STRING_ARRAY_FUNCTIONS_H_INCLUDED

#include <stdarg.h>
#include <stddef.h>

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
 * This function remove X characters at index from a string
 * The string needs to be allocated dynamically
 * @param str
 * @param startPosition, where to start the removal
 * @param length, the number of characters to remove
 * @return 1 for success, 0 for failure
 */
int removeChars (char *str, int startPosition, int length);

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

/**
 * Retrieve char index in a string
 * @param str the string
 * @param c the char we are looking for
 * @return an index >= 0 or -1 if not found
 */
long getCharIndex (char *str, char c);

/**
 * Substring a string
 * @param str original string
 * @param substr where the substring will be stored
 * @param start index where to start
 * @param length numbers of chars to substring
 * @return 1 for success, 0 for failure
 */
int substring (char *str, char *substr, int start, size_t length);

/**
 * Count the total length of all strings
 * @param strings
 * @param a varargs list, this list needs to be started before
 * @return the length
 */
size_t countStringsLength(int strings, va_list list);

/**
 * Concat strings into one
 * The generated string needs to be free
 * @params number of strings to concat
 * @param a list of strings
 * @return NULL if error, the concatened string if it succeeded
 */
char *concat (int strings, ...);

/**
 * Uppercase a string, the string needs to be allocated dynamically
 * Example : "bla" -> "BLA"
 * @param str
 * @return the uppercase string or null for error
 */
char *toUpperCase(char *str);

/**
 * Lowercase a string, the string needs to be allocated dynamically
 * Example : "BLA" -> "bla"
 * @param str
 * @return the lowercase string or null for error
 */
char *toLowerCase(char *str);

/**
 * Check if a string starts with another string
 * @param str
 * @param prefix
 * @param sensitivity 0 for case-sensitive,  1 for case-non-sensitive
 * @return 1 for true, 0 for false
 */
int startsWith(char *str, char *prefix, int sensitivity);

/**
 * Check if a string ends with another string
 * @param str
 * @param suffix
 * @return 1 for true, 0 for false
 */
int endsWith(char *str, char *suffix);

/**
 * Get index where a substring is in a string
 * @param str
 * @param substr
 * @param sensitivity 1 for case insensitivity
 * @return an index
 */
long getSubstringIndex(char *str, char *substr, int sensitivity);

/**
 * Check if strings are equals
 * @param str1
 * @param str2
 * @param sensitivity 1 for case insensitivity
 * @return 1 for true, 0 for false
 */
int areStringsEquals(char *str1, char *str2, int sensitivity);


/**
 * Check if a string is inside an array of string
 * @param str
 * @param array
 * @param arraySize
 * @return 1 for true, 0 for false
 */
int stringIntoArray(char *str, char **array, int arraySize);

/**
 * Add a string into an array
 * @param string
 * @param array
 * @param arraySize
 * @return 1 for success, 0 for failure
 */
int addStringIntoArray(char *string, char ***array, size_t arraySize);

/**
 * Remove an element at an index in an array
 * @param array
 * @param arraySize
 * @param index
 * @param freeElement if we free
 */
void removeElementAtIndex(char ***array, int arraySize, int index, int freeElement);

/**
 * Make string array deep copy
 * @param columns
 * @param columnsCounter
 * @return copied array
 */
char **makeStringsDeepCopy(char **columns, int columnsCounter);

#endif // STRING_ARRAY_FUNCTIONS_H_INCLUDED

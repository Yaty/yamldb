/*
**  Filename : directory_functions.c
**
**  Made by : Vincent GU�NIN ESGI - 3AL-1
**
**  Description : Contains directory manipulation related functions
*/

/*================ INCLUDES ================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "../header/general.h"
#include "../header/string_array_functions.h"
#include "../header/directory_functions.h"

/*================ FUNCTIONS ================*/
/*
Goal : Returns the content of a directory
Input : - incomeArrayLength (short*), length of incomeArray (char***)
        - incomeArray (char***), array to fill. At the end of the function, it
            contains the list of directory and files contained in th directory
            pointed by path
        - path (char*), path of the directory we want to get content
Output : char, state of the function :
            - 0, success
            - 1, error while opening the directory
            - 2, error while memory allocation
Require : - income_array (char***) needs to be free.
          - income_array (char***) needs to be initialized - malloc(0) works
*/
char getDirectoryContent(short *incomeArrayLength, char ***incomeArray, char *path) {
    DIR *directory = opendir(path);

    if( directory == NULL ) {
        return 1;
    }

    short length = *incomeArrayLength;
    char **array = *incomeArray;
    struct dirent *ent;

    while ((ent = readdir(directory)) != NULL) {
        //Si le nom du r�pertoire/fichier n'est ni "." ni ".."
        if( strcmp(ent->d_name, ".") && strcmp(ent->d_name, "..") ) {

            if( increaseStrArraySize(70, &length, 1, &array) == 0 ) { //Malloc error
                freeStrArray(length, array);
                return 2;
            }
            strcpy(array[length - 1], ent->d_name);
        }
    }

    closedir(directory);
    *incomeArray = array;
    *incomeArrayLength = length;

    return 0;
}

/*
Goal : Returns the list of sub-directory into a directory given in parameter.
Input : - incomeArrayLength (short*), length of incomeArray (char***)
        - incomeArray (char***), array to fill.
        - path (char*), path of the directory we want to list.
Output : char, state of the function :
            - 0, success
            - 1, error while opening the directory
            - 2, error while memory allocation
Require : - income_array (char***) needs to be free.
          - income_array (char***) needs to be initialized - malloc(0) works
*/
char getDirInDirectory(short *incomeArrayLength, char ***incomeArray, char* path) {
    char funcState;

    funcState = getDirectoryContent(incomeArrayLength, incomeArray, path);
    if( funcState != 0 ) {
        return funcState;
    }

    short resultArrayLength = 0;
    char **resultArray = malloc( resultArrayLength ); //Initializing with 0
    char **tempArray = *incomeArray;
    short counter;

    for( counter = 0; counter < *incomeArrayLength; counter++ ) {
        if( strchr( tempArray[counter], '.' ) == NULL ) { //S'il n'y a pas de '.' dans le nom de fichier (c'est un r�pertoire)
            if( increaseStrArraySize(70, &resultArrayLength, 1, &resultArray) == 0 ) { //Malloc error
                //Free all string array
                freeStrArray(*incomeArrayLength, tempArray);
                freeStrArray(resultArrayLength, resultArray);
                return 2;
            }
            strcpy(resultArray[resultArrayLength - 1], tempArray[counter]);
        }
    }

    *incomeArray = resultArray;
    *incomeArrayLength = resultArrayLength;
    return 0;
}

/*
Goal : Returns the list of files into a directory given in parameter.
Input : - incomeArrayLength (short*), length of incomeArray (char***)
        - incomeArray (char***), array to fill.
        - path (char*), path of the directory we want to list.
Output : char, state of the function :
            - 0, success
            - 1, error while opening the directory
            - 2, error while memory allocation
Require : - income_array (char***) needs to be free.
          - income_array (char***) needs to be initialized - malloc(0) works
*/
char getFilesInDirectory(short *incomeArrayLength, char ***incomeArray, char* path) {
    char funcState;

    funcState = getDirectoryContent(incomeArrayLength, incomeArray, path);
    if( funcState != 0 ) {
        return funcState;
    }

    short resultArrayLength = 0;
    char **resultArray = malloc( resultArrayLength ); //Initializing with 0
    char **tempArray = *incomeArray;
    short counter;

    for( counter = 0; counter < *incomeArrayLength; counter++ ) {
        if( strchr( tempArray[counter], '.' ) != NULL ) { //S'il y a un '.' dans le nom (c'est un fichier)
            if( increaseStrArraySize(70, &resultArrayLength, 1, &resultArray) == 0 ) { //Malloc error
                //Free all string array
                freeStrArray(*incomeArrayLength, tempArray);
                freeStrArray(resultArrayLength, resultArray);
                return 2;
            }
            strcpy(resultArray[resultArrayLength - 1], tempArray[counter]);
        }
    }

    *incomeArray = resultArray;
    *incomeArrayLength = resultArrayLength;
    return 0;
}
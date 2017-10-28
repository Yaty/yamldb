/*
**  Filename : file_manager.c
**
**  Made by : Vincent GU�NIN ESGI - 3AL-1
**
**  Description : Contains the functions for files using
*/

/* INCLUDES */
#include <stdio.h>
#include "../header/file_manager.h"

/*================ FUNCTIONS ================*/
/*
Goal : Check if a file exist
Input : filename (char*), path of the file we want to check if exist
Output : char (1 byte) :
            - 0, file doesn't exist
            - 1, file exist
*/
char fileExist(char* filename) {
    FILE* pf = fopen(filename, "r");

    if( pf == NULL ) { //Le fichier n'existe pas
        return 0;
    }else{ //Le fichier existe
        fclose(pf);
        return 1;
    }
}

/*
Goal : Create a file
Input : filename (char*), path of the file we want to create
Output : char (1 byte) :
            - 0, file already exist
            - 1, error while creating the file
            - 2, file as been successfully created
*/
char createFile(char* filename) {
    FILE* pf;

    if( fileExist(filename) ) { //Le fichier existe deja
        return 0;
    }

    pf = fopen(filename, "w");
    if( pf == NULL ) { //Le fichier n'a pas �t� cr��
        return 1;
    }

    return 2;
}

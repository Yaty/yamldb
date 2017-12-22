/*
**  Filename : file_manager.c
**
**  Made by : Vincent GU�NIN ESGI - 3AL-1
**
**  Description : Contains the functions for files using
*/

/* INCLUDES */
#include <stdio.h>
#include <dirent.h>
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
        name (char*), name of the bdd to create folder with same same that file.yaml
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

    pf = fopen(filename, "w+");
    if( pf == NULL ) { //Le fichier n'a pas �t� cr��
        return 1;
    }
    fclose(pf);
    return 2;
}

/*
Goal : Delete a file
Input : filename (char*), path to the file to delete
Output : short, state of the process
            - 0, success
            - 1, file does not exist
            - 2, error while deleting the file
*/
short deleteFile(char* filename) {
    if( !fileExist(filename) ) {
        return 1;
    }

    if( remove(filename) ) {
        return 2;
    }

    return 0;
}

/*
Goal : Check if a file is empty
Input : path (char*), path to the file
Output : short, state of the process
            - sizeOfFile
*/
int fileIsEmpty(char * path) {
    FILE * pf;
    int sizeOfFile;

    if( fileExist(path)){
        pf = fopen(path, "a");
        if(pf){
            fseek(pf, 0, SEEK_END);
            sizeOfFile = (int) ftell(pf);
            fclose(pf);
            return sizeOfFile;
        }
        return NULL;
    }
    return NULL;
}

/**
 * Check if a folder exists or not
 * @param path the folder path
 * @return 1 if it exists, 0 otherwise
 */
int isFolderExists(char *path) {
    if (path) {
        DIR* dir = opendir(path);

        if (dir) {
            closedir(dir);
            return 1;
        }
    }

    return 0;
}
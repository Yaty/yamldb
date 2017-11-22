/*
**  Filename : main.c
**
**  Made by : Hugo DA ROIT ESGI - 3AL-1
**
**  Description : main of the project
**
**  Goal of the project : Create a database manager
**      with a SQL interpreter and a GTK graphical interface
*/

#include <stdio.h>
#include <stdlib.h>
#include "../header/db_manager.h"
#include "../header/general.h"
#include "../header/string_array_functions.h"
#include "../header/directory_functions.h"
#include "../header/db_manipulation.h" //Test Vincent

int main (int argc, char **argv) {
    /*
    //sizeOfBiggestSlotOfStrArray() Example
    short length = 3;
    char *array[3] = {"Hello", "World", "C is great"};
    printf("Taille max : %d\n", sizeOfBiggestSlotOfStrArray(length, array));
    */
    /*
    //displayMenu() Example
    short length = 5;
    char *array[5] = {"Hello", "World", "C is great", "abcdefghijklmnopqrstuvwxyz", "yolooo"};
    displayMenu(length, array);
    */
    /*
    //getDirInDirectory() and getFilesInDirectory() Examples
    char** array;
    short arrayLength = 0;
    short counter;
    char funcState;

    array = malloc(0); //Initializing the pointer
    //funcState = getDirInDirectory(&arrayLength, &array, "src\\");
    funcState = getFilesInDirectory(&arrayLength, &array, "src\\");

    if( funcState == 1 ) { //Erreur lors de l'ouverture du r�pertoire
        printf("Impossible d'ouvrir le repertoire demande. Verifier le chemin donne.\n");
        return EXIT_FAILURE;
    }

    if( funcState == 2 ) { //Erreur lors de l'allocation m�moire
        printf("Une erreur s'est produite. Il est possible que la RAM de votre ordinateur soit insuffisante pour le traitement demande.\n");
        return EXIT_FAILURE;
    }

    printf("Contenu du repertoire : \n");

    for( counter = 0; counter < arrayLength; counter++ ) {
        printf("%s\n", array[counter]);
    }

    freeStrArray(arrayLength, array);
    */
    /*
    //concatenateSeveralStr() Example
    short length = 3;
    //char* strings[] = {"Hello", " world", " ici Vincent Guenin"};
    char* strings[] = {"Hello", " world", " !"};
    char concatenateStr[20];
    short funcState;
    funcState = concatenateSeveralStr(20, concatenateStr, length, strings);
    printf("funcState : %hd\n", funcState);
    printf("res : %s\n", concatenateStr);
    */
    /*
    //dirExist() Example
    char path[100] = "resources\\";
    char dirName[100] = "Base_3";
    char dirName2[100] = "Base_4";

    printf("dirExist(\"%s\", \"%s\") : %hd\n", path, dirName, dirExist(path, dirName));
    printf("dirExist(\"%s\", \"%s\") : %hd\n", path, dirName2, dirExist(path, dirName2));
    */

    //databaseManager();

    //createYamlFile();
    testVincent();

    return EXIT_SUCCESS;
}


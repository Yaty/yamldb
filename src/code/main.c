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
#include "../header/sql/insert.h"
#include "../header/db_manager.h"
#include "../header/general.h"
#include "../header/string_array_functions.h"
#include "../header/directory_functions.h"
#include "../header/db_manipulation.h" //Test Vincent
#include "../header/sql/query.h"
#include "../header/sql/utils.h"

#include <gtk/gtk.h>
#include "../header/gui/window_manager.h"

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

    /*QueryResult *res = SQLExecuteQuery("SELECT * FROM customers JOIN orders ON customers.customerId = orders.customerId JOIN orderDetails ON orders.orderId = orderDetails.orderId JOIN products ON orderDetails.productId = products.productId", "./resources/data/base1");
    SQLPrintQueryResult(res);
    SQLFreeQueryResult(res);*/
    //SQLExecuteQuery("INSERT INTO table (cityId, cityName, cityCountry) VALUES ('VALEUR_1', 'VALEUR_2', 'VALEUR_3')", "D:\\git\\CBDD1\\resources\\base1\\cities\\metadata.yml");
    //executeQueryInsert("insert into table values ('valeur 1', 'valeur 2')", "on s'en fou");

    //databaseManager();
    guiMain(argc, argv);

    return EXIT_SUCCESS;
}


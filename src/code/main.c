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

    databaseManager();

    return EXIT_SUCCESS;
}


/*
**  Filename : db_manager.c
**
**  Made by : Vincent GUÉNIN ESGI - 3AL-1
**
**  Description : Contains the database management functions
*/

/*================ TODO ================*/
//Open a database
//Create a table
//Drop database
//Drop table
//Give a list of the existing database ?

/*================ INCLUDES ================*/
#include "../header/db_manager.h"
#include "../header/general.h"
#include "../header/file_manager.h"
#include <windows.h>
#include <stdio.h>

/*================ FUNCTIONS ================*/
/*
Goal : Manage the database
Input : void
Output : void
*/
void databaseManager(void) {
    short menu;
    do{
        menu = displayDatabaseManagerMenu();

        system("cls");

        switch( menu ) {
        case 0: //Quitter le programme
            return;
        case 1: //Créer une bdd
            createDatabaseManager();
            break;
        case 2: //Ouvrir une bdd

            break;
        }

        system("cls");

    }while( menu != 0 );
}

/*
Goal : display the menu for the database manager
Input : void
Output : char, choice of the user in the menu
*/
char displayDatabaseManagerMenu(void) {
    short length = 3;
    short choice;
    char *array[3] = {"Quitter le programme", "Creer une base de donnee", "Ouvrir une base de donnee"};

    do{
        displayMenu(length, array);
        printf("\nVotre choix : ");
        scanf("%hd", &choice);

        if( choice < 0 || choice > length ) {
            printf("Valeur non valide, Reessayer\n");
            system("pause");
            system("cls");
        }

    }while( choice < 0 || choice > length );

    return choice;
}

/*
Goal : Create a database
Input : name (char*), name of the database to create
Output : void
*/
void createDatabase(char* name) {
    char filename[255];

    //Create filename
    //strcpy(filename, "..\\..\\resources\\db\\"); //Pour lancement depuis /bin/Debug/CBDD1.exe
    strcpy(filename, "resources\\db\\"); //Pour lancement depuis codeblocks
    strcat(filename, name);
    strcat(filename, ".yml");

    switch( createFile(filename) ) {
    case 0:
        printf("Cette base de donnee existe deja.\n");
        break;
    case 1:
        printf("Erreur lors de la creation de la base de donnee.\nL'emplacement ne doit pas etre accessible\n");
        break;
    case 2:
        printf("La base de donnee a ete creee avec succes.\n");
        break;
    }
    system("pause");
    system("cls");
}

/*
Goal : Create a database manager
Input : void
Output : void
*/
void createDatabaseManager(void) {
    char dbName[70];
    short length;

    do{ //Demander le nom de la base
        printf("Entrer le nom de la base de donnees a creer : ");
        getInput(70, dbName);

        length = strlen(dbName);

        if( length <= 0 ) { //Y a-t'il d'autres conditions d'erreur ?
            printf("Le nom entre n'est pas valide.\n");
            system("pause");
            system("cls");
        }
    }while( length <= 0 );

    //Créer la base
    createDatabase(dbName);
}

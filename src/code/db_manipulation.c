/*
**  Filename : db_manipulation.c
**
**  Made by : Vincent GUÉNIN ESGI - 3AL-1
**
**  Description : Contains the database manipulation functions
*/

/*================ TODO ================*/
//Create a table
//Drop database
//Drop table

/*================ INCLUDES ================*/
#include <stdio.h>
#include <windows.h>
#include "../header/db_manipulation.h"
#include "../header/general.h"

/*================ FUNCTIONS ================*/
/*
Goal : Manage the database's manipulations
Input : db (char*), name of the database we manipulate
Output : void
*/
void databaseManipulationManager(char *db) {
    short menu;
    do{
        menu = databaseManipulationManagerMenu();

        system("cls");

        switch( menu ) {
        case 0: //Quitter le programme
            return;
        case 1: //Créer une table

            break;
        case 2: //Ouvrir une table

            break;
        case 3: //Lister toutes les tables

            break;
        case 4: //Supprimer une base de données

            break;
        }

        system("cls");

    }while( menu != 0 );
}

/*
Goal : display the menu for the database manipulation
Input : void
Output : short, choice of the user in the menu
*/
short databaseManipulationManagerMenu(void) {
    short length = 5;
    short choice;
    char *array[] = {"Quitter le programme", "Creer une table", "Ouvrir une table", "Lister toutes les tables", "Supprimer une base de donnees"};

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
Goal : Create a table
Input : db (char*), name of the db we manipulate
Output : void
*/
void createTable(char *db) {

}

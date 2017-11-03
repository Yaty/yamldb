/*
**  Filename : table_manipulation.h
**
**  Made by : Alexis BENARD ESGI - 3AL-1
**
**  Description : Contains the table management functions
*/
/*================ INCLUDES ================*/
#include "../header/file_manager.h"
#include "../header/general.h"
#include "../header/table_manipulation.h"
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

/*================ TODO ================*/
//Drop table

/*================ FUNCTIONS ================*/
/*
Goal : Manage the table's manipulations
Input : - dbName (char*), name of the database we manipulate
        - tableName (char*), name of the table we manipulate
Output : void
*/
void tableManipulationManager(char *dbName, char *tableName) {
    short menu;

    do{
        menu = tableManipulationManagerMenu(dbName, tableName);
        system("cls");

        switch( menu ) {
        case 0: //Quitter le programme
            return;
        case 1: //Supprimer la table
            dropTable(dbName, tableName);
            printf("Supprimer la table.\n");
            system("pause");
            system("cls");
            return;
        }

        system("cls");

    }while( menu != 0 );
}

/*
Goal : display the menu for the table manipulation
Input : - dbName (char*), name of the database
        - tableName (char*), name of the table
Output : short, choice of the user in the menu
*/
short tableManipulationManagerMenu(char *dbName, char *tableName) {
    short length = 2;
    short choice;
    char *array[] = {"Quitter le programme", "Supprimer la table"};
    char title[100];
    short strLength = 3;
    char *str[] = {dbName, "/", tableName};

    concatenateSeveralStr(100, title, strLength, str);

    do{
        system("cls");
        displayMenu(title, length, array);
        printf("\nVotre choix : ");
        scanf("%hd", &choice);

        if( choice < 0 || choice > length ) {
            printf("Valeur non valide, Reessayer\n");
            system("pause");
        }

    }while( choice < 0 || choice > length );

    return choice;
}

/*
Goal : Suppress a table from a database
Input : - dbName (char*), name of the database
        - tableName (char*), name of the table we want to suppress
Input : short, state of the process
            - 0, success
            - 1, error while deleting the table's file
*/
short dropTable(char *dbName, char *tableName) {
    char path[15] = "resources\\";
    short strLength = 5;
    char *str[] = {path, dbName, "\\", tableName, ".yml"};
    char tablePath[255];

    concatenateSeveralStr(255, tablePath, strLength, str);

    if( deleteFile(tablePath) != 0 ) { //Supprime le fichier correspondant à la table
        return 1;
    }

    //Supprime le fichier correspondant à la structure de la table


    //Supprime la ligne correspondant à la table dans le fichier de la db
    deleteTableInDb(dbName, tableName);

    return 0;
}

/*
Goal : Delete the line that correspond to the table to suppress in the db's file
Input : - dbName (char*), name of the database
        - tableName (char*), name of the table to suppress
Output : void
*/
void deleteTableInDb(char *dbName, char *tableName) {

}

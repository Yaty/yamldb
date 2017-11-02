/*
**  Filename : table_manager.h
**
**  Made by : Alexis BENARD ESGI - 3AL-1
**
**  Description : Contains the table management functions
*/

/*================ INCLUDES ================*/
#include "../header/file_manager.h"
#include "../header/general.h"
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

/*================ FUNCTIONS ================*/

/*
Goal : Create a table in database folder
Input : db (char*), name of the db we manipulate
        tableName(char*), name of the table
Output : int
*/
int createTableFile(char *db, char *tableName){
    char tableFileName[500];

    strcpy(tableFileName, "resources\\");
    strcat(tableFileName, db);
    strcat(tableFileName, "\\");
    strcat(tableFileName, tableName);
    strcat(tableFileName, ".yml");

    switch( createFile(tableFileName) ) { //création de la table dans le fichier de base de données
    case 0:
        printf("Cette table existe deja.\n");
        return 0;
    case 1:
        printf("Erreur lors de la creation de la table.\nL'emplacement ne doit pas etre accessible\n");
        return 1;
    case 2:
        printf("La table a ete creee avec succes.\n");
        return 2;
    default:
        return 0;
    }
}

/*
Goal : Create a table
Input : db (char*), name of the db we manipulate
Output : void
*/
void createTable(char *db) {
    char tableName[100];
    char filename[255];
    FILE * pf;

    strcpy(filename, "resources\\"); //Pour lancement depuis codeblocks
    strcat(filename, db);
    strcat(filename, ".yml");

    if( pf = fopen(filename, "a") ) { //ouverture du fichier en ajout
        fseek(pf, 0, SEEK_END);
        if( ftell(pf) == 0 ){
            fputs("Tables :\n", pf);
        }
        printf("Saisir le nom de la table a creer : ");
        getInput(100, tableName);

        if( createTableFile(db, tableName) == 2 ){ //Si la table existe pas déjà
            fprintf(pf, "%s\n", tableName);
        }

    }else{
        printf("La base de donnees n'a pas ete trouvee\n");
    }
    fclose(pf);
    system("pause");
}

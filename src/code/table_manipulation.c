/*
**  Filename : table_manipulation.h
**
**  Made by : Alexis BENARD ESGI - 3AL-1
**
**  Description : Contains the table management functions
*/
/*================ INCLUDES ================*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../header/yaml/api.h"
#include "../header/file_manager.h"
#include "../header/general.h"
#include "../header/table_manipulation.h"

#if _WIN32
#define CLEAR "cls"
#define PAUSE "pause"
#else
#define CLEAR "clear"
#define PAUSE "echo \"Please enter a character to continue ...\" && read _"
#endif

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
        system(CLEAR);

        switch( menu ) {
        case 0: //Quitter le programme
            return;
        case 1: //Supprimer la table
            dropTable(dbName, tableName);
            printf("Supprimer la table.\n");
            system(PAUSE);
            system(CLEAR);
            return;
        }

        system(CLEAR);

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
        system(CLEAR);
        displayMenu(title, length, array);
        printf("\nVotre choix : ");
        scanf("%hd", &choice);

        if( choice < 0 || choice > length ) {
            printf("Valeur non valide, Reessayer\n");
            system(PAUSE);
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

    if( deleteFile(tablePath) != 0 ) { //Supprime le fichier correspondant � la table
        return 1;
    }

    //Supprime le fichier correspondant � la structure de la table


    //Supprime la ligne correspondant � la table dans le fichier de la db
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

/*
Goal : Add columns to table
Input : - dbName (char*), name of the database
        - table file (FILE*)
Output : void
*/
void addColumns(char *db, char *tableName){
    int columnNumberVal = columnNumber();
    int tempColumn = 0;
    short menu;
    Node * columnsNode;

    do{
        system(CLEAR);
        printf("Avancement : %d sur %d colonnes nommee.\n\n", tempColumn, columnNumberVal);
        menu = columnManipulationManagerMenu();
        system(CLEAR);

        switch( menu ) {
        case 0: //Saisir une colonne
            columnName(tempColumn, columnsNode);
            tempColumn++;
            break;
        case 1: //Ajouter une colonne
            columnNumberVal++;
            break;
        }
        YAMLSaveNode(columnsNode, tableName);
        system(CLEAR);

    }while( columnNumberVal != tempColumn );
}

/*
Goal : return column number in table
Input : nothing
Output : columns number (int)
*/
int columnNumber() {
    int columnNumber;

    do{
        fflush(stdin);
        printf("Indiquez le nombre de colonnes que vous voulez mettre dans le table : ");
        scanf("%d", &columnNumber);
    }while(columnNumber <= 0 || columnNumber > 2000 );

    return columnNumber;
}
/*
Goal : display the menu column creation
Input : nothing
Output : short, choice of the user in the menu
*/
short columnManipulationManagerMenu() {
    short length = 2;
    short choice;
    char *array[] = {"Saisir le nom d'une colonne", "Ajouter une colonne"};
    char title[100] = "Gestion des colonnes";

    do{
        fflush(stdin);
        displayMenu(title, length, array);
        printf("\nVotre choix : ");
        scanf("%hd", &choice);

        if( choice < 0 || choice > length ) {
            printf("Valeur non valide, Reessayer\n");
            system(PAUSE);
        }

    }while( choice < 0 || choice > length );

    return choice;
}

/*
Goal : Add column name at file table
Input : - dbName (char*), name of the database
        - table file (FILE*)
        - column number (int*)
Output : void
*/
void columnName(int incomeColumnNumber, Node *columnsNode){
    char columnNameStr[255];
    char columnType[50];
    int length = 0;

    system(CLEAR);
    printf("Saisir le nom de la colonne %d : ", incomeColumnNumber + 1);
    fflush(stdin);
    do{
        getInput(255, columnNameStr);
        length = strlen(columnNameStr);

        if( length <= 0 ) { //Y a-t'il d'autres conditions d'erreur ?
            printf("Le nom entre n'est pas valide.\n");
            system(PAUSE);
            system(CLEAR);
        }
    }while( length <= 0 );
    strcpy(columnType, selectColumnType());
    columnsNode = YAMLGetMapNode("Structure");
    YAMLAddValueChild(columnsNode, columnNameStr, columnType);
}

/*
Goal : allow to select type of the current column
Input : nothing
Output : char type
*/
char *selectColumnType(){
    short menu;

    menu = typeManipulationManagerMenu();
    system(CLEAR);

    switch( menu ) {
    case 0: //INT
        return "INT";
        break;
    case 1: //CHAR
        return "VARCHAR";
        break;
    case 2: //DATE
        return "DATE";
        break;
    case 3: //FLOAT
        return "FLOAT";
        break;
    case 4: //DOUBLE
        return "DOUBLE";
        break;
    case 5: //DATE
        return "DATE";
        break;
    case 6: //CHAR
        return "CHAR";
        break;
    case 7: //TIMESTAMP
        return "TIMESTAMP";
        break;
    default:
        return NULL;
    }
    system(CLEAR);
}

/*
Goal : display the menu type
Input : nothing
Output : short, choice of the user in the menu
*/
short typeManipulationManagerMenu(){
    char *array[] = {"INT", "VARCHAR", "DATE", "FLOAT", "DOUBLE", "DATE", "CHAR", "TIMESTAMP"};
    short length = 8;
    char title[100] = "Type de la colonne";
    short choice;

    do{
        fflush(stdin);
        displayMenu(title, length, array);
        printf("\nVotre choix : ");
        scanf("%hd", &choice);

        if( choice < 0 || choice > length ) {
            printf("Valeur non valide, Reessayer\n");
            system(PAUSE);
        }

    }while( choice < 0 || choice > length );

    return choice;
}

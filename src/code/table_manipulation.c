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
#include <unistd.h>
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
            printf("%d\n", dropTable(dbName, tableName)); //TEST
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
            - 1, error while deleting the table's struct file
            - 2, error while deleting the table's file
*/
short dropTable(char *dbName, char *tableName) {
    char path[15] = "resources\\";
    short strLength = 4;
    char *str[] = {path, dbName, "\\", tableName};
    char metadataPath[255];
    char dataPath[255];
    char dirPath[255];

    concatenateSeveralStr(255, dirPath, strLength, str);
    concatenateSeveralStr(255, metadataPath, strLength, str);
    concatenateSeveralStr(255, dataPath, strLength, str);
    strcat(metadataPath, "\\metadata.yml");
    strcat(dataPath, "\\data.yml");

    if( deleteFile(metadataPath) == 2 ) { //Supprime le fichier correspondant à la structure de la table
        return 1;
    }

    //Supprime le fichier correspondant à la table
    if( deleteFile(dataPath) == 2 ) {
        return 2;
    }

    //Supprime le répertoire correspondant à la table
    //deleteDirectory(dirPath);
    printf("%d\n", rmdir(dirPath));

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
    char* str[] = {"resources\\", dbName, ".yml"};
    char dbPath[255];
    int counter;

    concatenateSeveralStr(255, dbPath, 3, str);

    Node *root = YAMLGetChildAtIndex( YAMLParseFile(dbPath), 0 );
    Node *currentTable, *name;

    for( counter = 0; counter < YAMLGetSize(root); counter++ ) { //Pour chaque table
        currentTable = YAMLGetChildAtIndex(root, counter);
        name = YAMLGetChildAtIndex(currentTable, 0);
        if( strcmp( YAMLGetValue(name), tableName ) ) { //Si on est sur la table
            //On supprime le noeud dans root
            YAMLRemoveChildAtIndex(root, counter);
            YAMLSaveNode(root, dbPath);
            YAMLFreeNode(name);
            YAMLFreeNode(currentTable);
            YAMLFreeNode(root);
            return;
        }
    }
}

/*
Goal : Add columns to table
Input : - dbName (char*), name of the database
        - table file (FILE*)
Output : void
*/
void addColumns(char *dbName, char *tableName){
    int columnNumberVal = columnNumber();
    int tempColumn = 0;
    short menu;
    char *path[] = {"resources", "/", dbName, "/", tableName, "/", "metadata.yml"};
    Node * columnsNode = YAMLGetMapNode("Structure");
    char metaPath[500];
    concatenateSeveralStr(500, metaPath, 7, path);

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
        system(CLEAR);

    }while( columnNumberVal != tempColumn );
    YAMLSaveNode(columnsNode, metaPath);
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
TODO : Interdire de rentrer un nom de colonne déjà existant
*/
void columnName(int incomeColumnNumber, Node *columnsNode){
    char columnNameStr[255];
    char columnType[50];
    int length = 0;
    Node *temp;
    Node *child;

    do{
        system(CLEAR);
        printf("Saisir le nom de la colonne %d : ", incomeColumnNumber + 1);
        fflush(stdin);
        getInput(255, columnNameStr);
        length = strlen(columnNameStr);

        if( length <= 0 ) { //Y a-t'il d'autres conditions d'erreur ?
            printf("Le nom entre n'est pas valide.\n");
            system(PAUSE);
            system(CLEAR);
        }
    }while( length <= 0 );
    do{
        strcpy(columnType, selectColumnType());
    }while( columnType == NULL );

    temp = YAMLGetMapNode(columnNameStr);
    child = YAMLGetValueNode("type", columnType);
    YAMLAddChild(temp, child);
    child = YAMLGetValueNode("primary", "no");
    YAMLAddChild(temp, child);
    child = YAMLGetValueNode("defaultValue", "no");
    YAMLAddChild(temp, child);
    child = YAMLGetValueNode("autoIncrement", "no");
    YAMLAddChild(temp, child);
    YAMLAddChild(columnsNode, temp);
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

    switch( menu ) { // TODO USE ENUM
    case 0: //INT
        return "INT";
    case 1: //VARCHAR
        return "VARCHAR";
    case 2: //CHAR
        return "CHAR";
    case 3: //FLOAT
        return "FLOAT";
    case 4: //DOUBLE
        return "DOUBLE";
    case 5: //TEXT
        return "TEXT";
    default:
        return NULL;
    }
}

/*
Goal : display the menu type
Input : nothing
Output : short, choice of the user in the menu
*/
short typeManipulationManagerMenu(){
    char *array[] = {"INT", "VARCHAR", "CHAR", "FLOAT", "DOUBLE", "TEXT"};
    short length = 6;
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
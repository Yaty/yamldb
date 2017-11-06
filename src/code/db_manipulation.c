/*
**  Filename : db_manipulation.c
**
**  Made by : Vincent GU�NIN ESGI - 3AL-1
**
**  Description : Contains the database manipulation functions
*/
/*================ INCLUDES ================*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "../header/db_manipulation.h"
#include "../header/general.h"
#include "../header/table_manipulation.h"
#include "../header/file_manager.h"
#include "../header/directory_functions.h"
#include "../header/string_array_functions.h"
//#include "../yaml/parser.c"

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

/*================ FUNCTIONS ================*/
/*
Goal : Manage the database's manipulations
Input : dbName (char*), name of the database we manipulate
Output : void
*/
void databaseManipulationManager(char *dbName) {
    short menu;
    do{
        menu = databaseManipulationManagerMenu(dbName);
        system(CLEAR);

        switch( menu ) {
        case 0: //Quitter le programme
            return;
        case 1: //Cr�er une table
            createTable(dbName);
            break;
        case 2: //Ouvrir une table
            openTableManager(dbName);
            break;
        case 3: //Lister toutes les tables
            displayTablesListManager(dbName);
            break;
        case 4: //Supprimer une base de donn�es
            dropDatabaseManager(dbName);
            return;
            break;
        }
        system(CLEAR);

    }while( menu != 0 );
}

/*
Goal : display the menu for the database manipulation
Input : dbName (char*), name of the database
Output : short, choice of the user in the menu
*/
short databaseManipulationManagerMenu(char *dbName) {
    short length = 5;
    short choice;
    char *array[] = {"Quitter le programme", "Creer une table", "Ouvrir une table", "Lister toutes les tables", "Supprimer une base de donnees"};

    do{
        system(CLEAR);
        displayMenu(dbName, length, array);
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
Goal : Remove all tables' files
Input : dirName (char*), path of the database to destroy
Output : short, state of the process :
            - 0, success
            - 1, error while opening the directory
            - 2, error while memory allocation
            - 3, error while deleting the file
*/
short deleteTableFiles(char *dirName) {
    char fileName[255];
    char **tablesName = malloc(0);
    short tablesNameLength = 0;
    short funcState;
    short counter;

    funcState = getFilesInDirectory(&tablesNameLength, &tablesName, dirName);
    if( funcState != 0 ) {
        return funcState;
    }

    for( counter = 0; counter < tablesNameLength; counter++ ) {
        strcpy(fileName, dirName);
        strcat(fileName, tablesName[counter]);
        funcState = deleteFile(fileName); //Supprime le fichier table
        if( funcState != 0 ) {
            return 3;
        }
    }

    //Free tablesName
    freeStrArray(tablesNameLength, tablesName);
    return 0;
}

/*
Goal : Drop a database
Input : dbName (char*), name of the database to destroy
Output : short, state of the function
            - 0, success
            - 1, error while opening the directory
            - 2, error while memory allocation
            - 3, error while deleting the table's file
            - 4, error while deleting the db's directory
            - 5, error while deleting the db's file
            - 6, error while concatening the strings
*/
short dropDatabase(char *dbName) {
    short dirLength = 100;
    char dirPath[15] = "resources\\";
    char dirName[dirLength];
    char fileName[255];
    short funcState;
    short str1Length = 3;
    char *str1[] = {dirPath, dbName, "\\"};
    short str2Length = 3;
    char *str2[] = {dirPath, dbName, ".yml"};

    if( concatenateSeveralStr(dirLength, dirName, str1Length, str1) != 0 ) {
        return 6;
    }

    if( dirExist(dirPath, dbName) == 1 ) { //Si le r�pertoire au nom de la base existe
        funcState = deleteTableFiles(dirName); //Supprime les fichiers table
        if( funcState != 0 ) {
            return funcState;
        }

        funcState = (short)rmdir(dirName); //Supprime le r�pertoire
        if( funcState != 0 ) {
            return 4;
        }
    }

    if( concatenateSeveralStr(255, fileName, str2Length, str2) != 0 ) {
        return 6;
    }

    funcState = (short)remove(fileName); //Supprime le fichier db
    if( funcState != 0 ) {
        return 5;
    }
    return 0;
}

/*
Goal : Manage the db suppression
Input : dbName (char*), name of the database to suppress
Output : short, state of the process
            - 0, success
            - other, error
*/
short dropDatabaseManager(char *dbName) {
    short funcState;

    funcState = dropDatabase(dbName);
    if( funcState == 0 ) {
        printf("La base de donnees a ete supprimee avec succes.\n");
    }else{
        printf("Erreur lors de la suppression de la base de donnees.\n");
    }
    system("pause");
    system(CLEAR);

    return funcState;
}

/*
Goal : Create a table in database folder
Input : db (char*), name of the db we manipulate
        tableName(char*), name of the table
Output : int
*/
int createTableFile(char *db, char *tableName){
    char tableFileName[255];
    short strLength = 5;
    char *str[] = {"resources\\", db, "\\", tableName, ".yml"};

    concatenateSeveralStr(255, tableFileName, strLength, str);

    switch( createFile(tableFileName) ) { //cr�ation de la table dans le fichier de base de donn�es
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
void createTable(char *db) { /* A modifier avec les fonctions de yml parsing + Ajouter structure de la table */
    char tableName[100];
    char filename[255];
    char tablePath[1000];
    FILE *pf;
    short strLength = 3;
    short strLengthTwo = 2;
    char *str[] = {"resources\\", db, ".yml"};
    char *strTable[] = {"resources\\", db};

    concatenateSeveralStr(255, filename, strLength, str);
    concatenateSeveralStr(255, tablePath, strLengthTwo, strTable);

    if( (pf = fopen(filename, "a")) ) { //ouverture du fichier en ajout
        fseek(pf, 0, SEEK_END);
        if( ftell(pf) == 0 ){
            fputs("Tables :\n", pf);
        }
        printf("Saisir le nom de la table a creer : ");
        getInput(100, tableName);
        if( createTableFile(db, tableName) == 2 ){ //Si la table existe pas d�j�
            fprintf(pf, "\t%s\n", tableName);
            strcat(tablePath, "\\");
            strcat(tablePath, tableName);
            strcat(tablePath, ".yml");
            fclose(pf);
            addColumns(db, tablePath);
        }

    }else{
        printf("La base de donnees n'a pas ete trouvee\n");
    }
    system("pause");
}

/*
Goal : Get the names of all tables created
Input : - dirName (char*), path of the db directory
        - tableNamesLength (short*), length of tableNames (char***)
        - tableNames (char***), array to fill.
Output : char, state of the treatment :
            - 0, success
            - 1, error while opening the directory
            - 2, error while memory allocation
Require : - tableNames (char***) needs to be free.
          - tableNames (char***) needs to be initialized - malloc(0) works
*/
char getTablesList(char *dirName, short *tableNamesLength, char ***tableNames) {
    char funcState;
    short counter;
    char *lastOcc;
    char **tempArray;
    int pos;

    funcState = getFilesInDirectory(tableNamesLength, tableNames, dirName);
    if( funcState != 0 ) {
        return funcState;
    }

    tempArray = *tableNames;

    //Pour chaque nom, on coupe le nom au dernier '.' pour r�cup�rer juste le nom de la table
    for( counter = 0; counter < *tableNamesLength; counter++ ) {
        lastOcc = getLastOccInStr(tempArray[counter], '.');
        pos = lastOcc - tempArray[counter]; //Nb de char avant le dernier '.'
        tempArray[counter][pos] = '\0';
    }

    *tableNames = tempArray;
    return 0;
}

/*
Goal : Get the names of all tables
Input : - dirName (char*), path of the database's directory
        - resultArrayLength (short*), length of resultArray.
        - resultArray (char***), at the end of the function, this array contains
            the name of every database.
Output : void
Require : - resultArray char*** needs to be free.
*/
void getTablesListManager(char *dirName, short *resultArrayLength, char ***resultArray) {
    char **array;
    short arrayLength = 0;
    char funcState;

    array = malloc(0); //Initializing the pointer
    funcState = getTablesList(dirName, &arrayLength, &array);

    if( funcState == 1 ) { //Erreur lors de l'ouverture du r�pertoire
        printf("Impossible d'ouvrir le repertoire contenant les tables.\n");
        return;
    }

    if( funcState == 2 ) { //Erreur lors de l'allocation m�moire
        printf("Une erreur s'est produite. Il est possible que la RAM de votre ordinateur soit insuffisante pour le traitement demande.\n");
        return;
    }

    *resultArray = array;
    *resultArrayLength = arrayLength;
}

/*
Goal : Display the tables' list
Input : dirName (char*), path of the database's directory
Output : void
*/
void displayTablesList(char *dirName) {
    char **list;
    short length;
    short counter;

    getTablesListManager(dirName, &length, &list);

    printf("Liste des tables creees :\n");
    for( counter = 0; counter < length; counter++ ) {
        printf("\t%s\n", list[counter]);
    }
}

/*
Goal : Manage the tables' list display
Input : dbName (char*), name of the database
Output : void
*/
void displayTablesListManager(char *dbName) {
    char dirName[255];
    short strLength = 3;
    char *str[] = {"resources\\", dbName, "\\"};

    concatenateSeveralStr(255, dirName, strLength, str);

    displayTablesList(dirName);
    system("pause");
    system(CLEAR);
}

/*
Goal : Manage the table opening
Input : void
Output : void
*/
void openTableManager(char *dbName) { //A tester en revenant
    char **list;
    short length;
    short choice;
    char table[70];
    char dirName[255];
    short strLength = 3;
    char *str[] = {"resources\\", dbName, "\\"};

    concatenateSeveralStr(255, dirName, strLength, str);
    getTablesListManager(dirName, &length, &list); //R�cup�rer la liste des noms

    choice = openTableAskNumber(length, list);
    if( choice == 0 ) {
        system(CLEAR);
        return;
    }

    system(CLEAR);
    strcpy(table, list[choice - 1]);
    tableManipulationManager(dbName, table);
}

/*
Goal : Ask the table to open
Input : - length (short), length of array
        - array (char**), list of the tables' name
Output : number of the table
*/
short openTableAskNumber(short length, char **array) {
    short counter;
    short choice;

    do{
        printf("Liste des tables :\n");

        printf("\t0. Retourner au menu precedent\n");
        for( counter = 0; counter < length; counter++ ) {
            printf("\t%hd. %s\n", counter + 1, array[counter]);
        }

        printf("Entrer le numero de la table a ouvrir : ");
        scanf("%hd", &choice);

        if( choice < 0 || choice > length ) {
            printf("Valeur non valide, Reessayer\n");
            system("pause");
            system(CLEAR);
        }

    }while( choice < 0 || choice > length );

    return choice;
}

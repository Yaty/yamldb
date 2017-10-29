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
#include "../header/directory_functions.h"
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
        case 3: //Lister les bases
            displayDatabasesList();
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
    short length = 4;
    short choice;
    char *array[4] = {"Quitter le programme", "Creer une base de donnees", "Ouvrir une base de donnees", "Lister toutes les bases de donnees"};

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
    //strcpy(filename, "..\\..\\resources\\"); //Pour lancement depuis /bin/Debug/CBDD1.exe
    strcpy(filename, "resources\\"); //Pour lancement depuis codeblocks
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

/*
Goal : Get the names of all databases created
Input : - dbNamesLength (short*), length of incomeArray (char***)
        - dbNames (char***), array to fill.
Output : char, state of the treatment :
            - 0, success
            - 1, error while opening the directory
            - 2, error while memory allocation
Require : - dbNames (char***) needs to be free.
          - dbNames (char***) needs to be initialized - malloc(0) works
*/
char getDatabasesList(short *dbNamesLength, char ***dbNames) {
    char funcState;
    short counter;
    char* lastOcc;
    char **tempArray;
    int pos;

    funcState = getFilesInDirectory(dbNamesLength, dbNames, "resources\\");
    if( funcState != 0 ) {
        return funcState;
    }

    tempArray = *dbNames;

    //Pour chaque nom, on coupe le nom au dernier '.' pour récupérer juste le nom de la bdd
    for( counter = 0; counter < *dbNamesLength; counter++ ) {
        lastOcc = getLastOccInStr(tempArray[counter], '.');
        pos = lastOcc - tempArray[counter]; //Nb de char avant le dernier '.'
        tempArray[counter][pos] = '\0';
    }

    *dbNames = tempArray;
    return 0;
}

/*
Goal : Get the names of all databases
Input : - resultArrayLength (short*), length of resultArray.
        - resultArray (char***), at the end of the function, this array contains
            the name of every database.
Output : void
Require : - resultArray char*** needs to be free.
*/
void getDatabasesListManager(short *resultArrayLength, char ***resultArray) {
    char **array;
    short arrayLength = 0;
    char funcState;

    array = malloc(0); //Initializing the pointer
    funcState = getDatabasesList(&arrayLength, &array);

    if( funcState == 1 ) { //Erreur lors de l'ouverture du répertoire
        printf("Impossible d'ouvrir le repertoire contenant les bases de donnees.\n");
        return;
    }

    if( funcState == 2 ) { //Erreur lors de l'allocation mémoire
        printf("Une erreur s'est produite. Il est possible que la RAM de votre ordinateur soit insuffisante pour le traitement demande.\n");
        return;
    }

    *resultArray = array;
    *resultArrayLength = arrayLength;
}

/*
Goal : Display the databases' list
Input : void
Output : void
*/
void displayDatabasesList(void) {
    char **list;
    short length;
    short counter;

    getDatabasesListManager(&length, &list);

    printf("Liste des bases de donnees creees :\n");
    for( counter = 0; counter < length; counter++ ) {
        printf("\t%s\n", list[counter]);
    }

    system("pause");
    system("cls");
}

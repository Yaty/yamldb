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

#if _WIN32
#define CLEAR "cls"
#define PAUSE "pause"
#else
#define CLEAR "clear"
#define PAUSE "echo \"Please enter a character to continue ...\" && read _"
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
            case 5:
                createAttribut(dbName);
                break;
            default:
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
    short length = 6;
    short choice;
    char *array[] = {"Quitter le programme", "Creer une table", "Ouvrir une table", "Lister toutes les tables", "Supprimer une base de donnees", "Definir des attributs"};

    do{
        system(CLEAR);
        displayMenu(dbName, length, array);
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
    system(PAUSE);
    system(CLEAR);

    return funcState;
}

/*
 * Goal : Init the created table file with the root node
 * Input : path (char*), path of the created file
 * Output : void
 */
void createTableFileInit(char* path) {
    Node *root = YAMLGetMapNode("structure");
    YAMLSaveNode(root, path);
    YAMLFreeNode(root);
}

/*
Goal : Create a table in database folder
Input : db (char*), name of the db we manipulate
        tableName(char*), name of the table
Output : int
*/
int createTableFile(char *db, char *tableName){
    char tableFileName[255];
    short strLength = 6;
    char *str[] = {"resources\\", db, "\\", tableName, "\\", "metadata.yml"};

    concatenateSeveralStr(255, tableFileName, strLength, str);

    switch( createFile(tableFileName) ) { //cr�ation de la table dans le fichier de base de donn�es
    case 0:
        printf("Cette table existe deja.\n");
        return 0;
    case 1:
        printf("Erreur lors de la creation de la table.\nL'emplacement ne doit pas etre accessible\n");
        return 1;
    case 2:
        //Init le premier noeud
        printf("La table a ete creee avec succes.\n");
        return 2;
    default:
        return 0;
    }
}

/*
 * Goal : Save the table into the yaml db file
 * Input :  - dbName (char*), name of the database
 *          - tableName (char*), name of the table
 * Output : void
 */
void addTableToDbFile(char* dbName, char* tableName) {
    Node *dbNode, *newTable, *name, *status;
    char dbFilePath[255];
    char *str[] = {"resources\\", dbName, ".yml"};

    concatenateSeveralStr(255, dbFilePath, 3, str);

    //dbNode = YAMLParseFile(str);
    dbNode = YAMLGetChildAtIndex( YAMLParseFile(dbFilePath), 0 );
    if( YAMLGetType(dbNode) == UNDEFINED ) {
        YAMLFreeNode(dbNode);
        dbNode = YAMLGetSequenceNode(strdup("Tables"));
    }
    newTable = YAMLGetSequenceValueNode();
    name = YAMLGetValueNode(strdup("name"), strdup(tableName));
    status = YAMLGetValueNode(strdup("status"), strdup("empty"));

    YAMLAddChild(newTable, name);
    YAMLAddChild(newTable, status);
    YAMLAddChild(dbNode, newTable);

    YAMLSaveNode(dbNode, dbFilePath);
    YAMLFreeNode(newTable);
    YAMLFreeNode(dbNode);
    addColumns(dbName, tableName);
}

/*
 * Goal : Get the name of the table to create
 * Input : tableName (char*), name of the table
 * Output : void
 */
void askTableName(char* tableName) {
    int length = 0;
    do{
        printf("Saisir le nom de la table a creer : ");
        fflush(stdin);
        getInput(100, tableName);
        length = strlen(tableName);

        if( length <= 0 ) { //Y a-t'il d'autres conditions d'erreur ?
            printf("Le nom entre n'est pas valide.\n");
            system(PAUSE);
            system(CLEAR);
        }
    }while( length <= 0 );
}

/*
Goal : Create a table
Input : db (char*), name of the database we manipulate
Output : void
*/
void createTable(char *db) {
    char tableName[100];
    char tablePath[255];
    char tableDirPath[255];

    //Saisir le nom de la table
    askTableName(tableName);

    //Crée le chemin du répertoire table
    char *strDb[] = {"resources\\", db, "\\", tableName};
    concatenateSeveralStr(255, tableDirPath, 4, strDb);

    //Créer le fichier de la table
    createDir(tableDirPath);
    if( createTableFile(db, tableName) == 2 ) { //Si la table a été créée avec succès
        //Créer le chemin du fichier
        char *strTable[] = {tableDirPath, "\\metadata.yml"};
        concatenateSeveralStr(255, tablePath, 2, strTable);

        //Ajouter le nom dans le fichier db
        addTableToDbFile(db, tableName);
    }
    system(PAUSE);
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

    funcState = getDirInDirectory(tableNamesLength, tableNames, dirName);
    if( funcState != 0 ) {
        return funcState;
    }

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
    system(PAUSE);
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
            system(PAUSE);
            system(CLEAR);
        }

    }while( choice < 0 || choice > length );

    return choice;
}

/*
 * Goal : Add a table to a database
 * Input : void
 * Output : void
 * */
void testAddTable() {
    Node *newTable;
    Node *name, *status;
    Node *table = YAMLParseFile("resources\\test_Vincent.yml");
    char tableName[50];

    table = YAMLGetChildAtIndex(table, 0);
    if( YAMLIsUndefined(table) ) { //Si le noeud parent a pour type undefined (ne contient aucun enfant)
        YAMLFreeNode(table);
        table = YAMLGetSequenceNode("tables"); //On récupère une nouvelle séquence
    }

    printf("Entrer le nom de la table a creer : ");
    fflush(stdin);
    getInput(50, tableName);

    newTable = YAMLGetSequenceValueNode();

    name = YAMLGetValueNode("name", tableName);
    status = YAMLGetValueNode("status", "empty");

    YAMLAddChild(newTable, name);
    YAMLAddChild(newTable, status);
    YAMLAddChild(table, newTable);

    YAMLSaveNode(table, "resources\\test_Vincent.yml");
    //YAMLFreeNode(name);
    //YAMLFreeNode(status);
    YAMLFreeNode(newTable);
    YAMLFreeNode(table);
}

/*
 * Goal : Create a Database and the associated file
 * Input : void
 * Output : void
 */
void testCreateDb() {
    Node *table = YAMLGetSequenceNode("tables");
    YAMLSaveNode(table, "resources\\test_Vincent.yml");
    YAMLFreeNode(table);
}

/*
 * Goal : Add attributs to columns line.
 * Input : str (char *)
 * Output : void
 * */
void createAttribut(char* dbName){
    char *array[] = {"resources", "\\", dbName};
    char **list;
    short length = 3;
    char path[255];
    short choice;
    char table[100];

    concatenateSeveralStr(255, path, length, array);
    getTablesListManager(path, &length, &list); //R�cup�rer la liste des noms

    choice = openTableAskNumber(length, list);
    if( choice == 0 ){
        system(CLEAR);
        return;
    }
    strcpy(table, list[choice - 1]);
    definedAttributs(dbName, table);
}

/*
 * Goal : defined attribut to column.
 * Input : str (char *)
 *         table (char *)
 * Output : void
 * */
void definedAttributs(char *dbName, char *table){
    char *array[] = {"menu precedent", "clef primaire", "Valeur par defaut", "Auto increment"};
    short length = 4;
    int choice;
    do{
        displayMenu(table, length, array);
        printf("Votre choix: ");
        scanf("%d", &choice);
        system(CLEAR);
    }while(choice < 0 && choice >= length);
    switch(choice){
        case 0:
            return;
        case 1:
            addCaracteristics(dbName, table, choice);
            break;
        case 2:
            addCaracteristics(dbName, table, choice);
            break;
        case 3:
            addCaracteristics(dbName, table, choice);
            break;
    }
}

/*
 * Goal : defined the primary key.
 * Input : dbName (char *)
 *         table (char *)
 * Output : void
 * */
void addCaracteristics(char *dbName, char *table, int type){
    char *array[] = {"resources", "/", dbName, "/", table, "/", "metadata.yml"};
    char path[255];
    char val[255];
    short length = 7;
    int choice;
    Node *result;
    Node *child;


    concatenateSeveralStr(255, path, length, array);
    Node *column = YAMLParseFile(path);
    result = YAMLGetChildByKey(column, "Structure");
    choice = displayColumns(result);
    if( type == 1 ){
        setCaracteristics(result, choice, "yes", path, type);
    }else if( type == 2 ){
        do{
            printf("Ecrire la valeur par defaut: ");
            getInput(255, val);
        }while(strcmp(val, "no") == 0);
        setCaracteristics(result, choice, val, path, type);
    }else if( type == 3 ){
        setCaracteristics(result, choice, "yes", path, type);
    }
    system(PAUSE);
    system(CLEAR);
}

/*
 * Goal : display columns from yaml file.
 * Input : colums (Node *)
 * Output : choice of column
 * */
int displayColumns(Node *columns){
    int i;
    int choice;
    char *value;
    Node *column;
    Node *child;

    for( i = 0; i < YAMLGetSize(columns); i++ ){
        column = YAMLGetChildAtIndex(columns, i);
        value = YAMLGetKey(column);
        printf("%d - %s\n", i, value);
    }
    do{
        flush();
        printf("Saisir le numéro de la colonne: ");
        scanf("%d", &choice);
    }while(choice >= YAMLGetSize(columns) || choice < 0);

    return choice;
}

/*
 * Goal : set values for columns caracteristics.
 * Input : result (Node *)
 *         choice (int)
 *         text (*char)
 *         path (*char)
 *         type (int)
 * Output : void
 * */
void setCaracteristics(Node *result, int choice, char *text, char *path, int type){
    Node *column;
    Node *child;

    column = YAMLGetChildAtIndex(result, choice);
    child = YAMLGetChildAtIndex(column, type);
    YAMLSetValue(child, text);
    YAMLSaveNode(result, path);
}
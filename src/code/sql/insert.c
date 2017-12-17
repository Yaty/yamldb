//
// Created by Hugo on 12/11/2017.
//

#include <stdlib.h>

#include <stdio.h>
#include <string.h>
#include "../../header/sql/parser.h"
#include "../../header/string_array_functions.h"
#include "../../header/sql/utils.h"
#include "../../header/yaml/api.h"

#if _WIN32
#define CLEAR "cls"
#define PAUSE "pause"
#else
#define CLEAR "clear"
#define PAUSE "echo \"Please enter a character to continue ...\" && read _"
#endif

/**
 * Allow to collect table name
 * char *query: request
 * char *space: This is the request before "VALUES" or ('col',...)
 * char *table: table name
 * return table name
 */
size_t tablesSizeAndName(char *query, char *space, char *table){
    size_t val;

    space = strchr(query, ' ');
    val = (space - query);
    strncpy(table, query, val);

    return val;
}

/**
 * allow to collect the column name
 * @param query
 * @param columns: array who contain columns names
 * @param nextVal: limits of the selection
 * @return number of columns
 */
int columnsSizeAndName(char *query, char **columns, size_t *nextVal){
    int counter;
    char *charac;
    char *tempQuery;

    charac = strchr(query, ')');
    *nextVal = (charac - query);
    tempQuery = malloc(sizeof(char*) * *nextVal);
    strncpy(tempQuery, query, *nextVal);
    *columns = getParams(tempQuery + 1, &counter);

    return counter;
}

/**
 * allow to know if the number of colums is bigger than number of values
 * @param counterColumns
 * @param counterValuesr
 * @param values
 */
void compareCounters( int counterColumns, int counterValues, char **values ){
    int i;

    if( counterValues > counterColumns || counterValues < counterColumns ){
        printf("La requete n'est pas valide 3.");
        system(PAUSE);
        return;
    }
}

/**
 *
 * @param values: value who need to change
 * @param childrenNumber: number of children
 * @param i: current position
 * @return
 */
char *modifyStr(char *values, int childrenNumber, int i){
    values = values + 1;
    if( i + 1 == childrenNumber ){
        values[strlen(values) - 2] = '\0';
    }else{
        values[strlen(values) - 1] = '\0';
    }
    return values;
}

/**
 *
 * @param res: query result
 * @param query
 * @param dbPath
 * @param dataPath
 */
void executeInsert(QueryResult *res, char *query, char *dbPath, char *dataPath) {
    char *space; //chaine + le dernier espace
    char table[200]; //Nom de la table
    char **columns; //colonnes
    char **values; //valeurs à inserer
    int counterColumns; //nombre de colonne
    int counterValues; //nombre de valeurs à insérer.
    int i;
    int j;
    int state;
    char *key = "data";
    size_t val;
    size_t nextVal;
    Node *sequence = YAMLParseFile(dataPath);
    Node *file = YAMLParseFile(dbPath);
    Node *add = YAMLGetSequenceValueNode();

    sequence = YAMLGetChildAtIndex(sequence, 0);
    if( YAMLIsUndefined(sequence) || sequence == NULL ){
        YAMLFreeNode(sequence);
        sequence = YAMLGetSequenceNode(key);
    }

    file = file->children;
    val = tablesSizeAndName(query, space, table);
    query += val + 1;
    if(startsWith(query, "values", 1)){
        query += 8;
        values = getParams(query, &counterValues);
        compareCounters(counterValues, file->childrenNumber, values);
        for(i = 0; i < file->childrenNumber; i++){
            values[i] = modifyStr(values[i], file->childrenNumber, i);
            YAMLAddChild(add, YAMLGetValueNode(file->children[i].key, values[i]));
        }
        YAMLAddChild(sequence, add);
        YAMLSaveNode(sequence, dataPath);
    }else if(startsWith(query, "(",1 )){
        counterColumns = columnsSizeAndName(query, &columns, &nextVal);
        query += nextVal + 2;
        if(startsWith(query, "values", 1)){
            query += 8;
            values = getParams(query, &counterValues);
            compareCounters( counterColumns, counterValues, values );
            for(i = 0; i < counterValues; i++){
                values[i] = modifyStr(values[i], counterValues, i);
            }
            for(i = 0; i < file->childrenNumber; i++){
                state = 0;
                for(j = 0; j < counterColumns; j++){
                    if( strcmp(file->children[i].key, columns[j]) == 0 ){
                        YAMLAddChild(add, YAMLGetValueNode(file->children[i].key, values[i]));
                        state = 1;
                        break;
                    }
                }
                if(state == 0){
                    YAMLAddChild(add, YAMLGetValueNode(file->children[i].key, "NULL"));
                }
            }
            YAMLAddChild(sequence, add);
            YAMLSaveNode(sequence, dataPath);
        }else{
            printf("La requete n'est pas valide 2.");
            system(PAUSE);
            return;
        }
    }else{
        printf("La requete n'est pas valide 1.");
        system(PAUSE);
        return;
    }
}


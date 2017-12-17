/*
**  Filename : general.c
**
**  Made by : Vincent GU�NIN ESGI - 3AL-1
**
**  Description : Contains varied easy functions
*/

/*================ INCLUDES ================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "../header/general.h"

/*================ FUNCTIONS ================*/
/*
Goal : Give the length of the longest slot of a str array
Input : - length (short), length of the array
        - array (char**), pointer of the array to process
Output : short :
            - -1 : Error while processing
            - other : length of the longest slot of the array

Example : - Input : - length : 3
                    - array : ["Hello", "World", "C is great"]
          - Output : 10 (length of "C is great")
*/
short sizeOfBiggestSlotOfStrArray(short length, char **array) {
    if( length <= 0 ) {
        return -1;
    }

    short counter;
    short maxLength = -1;
    short currentLength;

    for( counter = 0; counter < length; counter++ ) {
        currentLength = strlen( array[counter] );

        if( currentLength > maxLength ) {
            maxLength = currentLength;
        }
    }

    return maxLength;
}

/*
Goal : Display title's menu
Input : - maxLength (short), length of the longest str
        - title (char*), title to display
        - leftChar (char), char to display on the left
        - rightChar (char), char to display on the right
Output : void
*/
void displayMenuTitle(short maxLength, char *title, char leftChar, char rightChar) {
    short counter;
    short titleLength;
    short totalLength;
    short gap;

    totalLength = maxLength + 4;
    titleLength = strlen(title);
    gap = (totalLength - titleLength) / 2;

    printf("%c", leftChar);
    for( counter = 0; counter < gap; counter++ ) { //Affiche les espaces � gauche
        printf(" ");
    }

    printf("%s", title); //Affiche le titre

    gap = totalLength - (gap + titleLength) + 1;
    for( counter = 0; counter < gap; counter++ ) { //Affiche les espaces � droite
        printf(" ");
    }
    printf("%c\n", rightChar);
}

/*
Goal : Display a menu
Input : - title (char*), Str with the title of the menu
        - length (short), length of the array
        - menuStr (char**), Str to display in the menu
Output : void
*/
void displayMenu(char *title, short length, char **menuStr) {
    short maxLength;
    short counter;
    short counter2;
    short spaceToWrite;
    char topChar = '-';
    char leftChar = '|';
    char rightChar = '|';
    char bottomChar = '-';
    short lengthNumber;

    //Take the length of the longest string to display
    maxLength = sizeOfBiggestSlotOfStrArray(length, menuStr);
    if( maxLength < strlen(title) ) {
        maxLength = strlen(title);
    }

    //Take the number of figure in length
    lengthNumber = countFigureInNumber(length);

    //Display top delimiter
    printf(" ");
    for( counter = 0; counter < maxLength + 4 + lengthNumber; counter++ ) {
        printf("%c", topChar);
    }
    printf("\n");

    //Diplay title
    displayMenuTitle(maxLength, title, leftChar, rightChar);

    for( counter = 0; counter < length; counter++ ) {
        printf("%c %hd. %s ", leftChar, counter, menuStr[counter]); //Display string

        //Display extra spaces
        spaceToWrite = maxLength - strlen(menuStr[counter]);
        for( counter2 = 0; counter2 < spaceToWrite; counter2++ ) {
            printf(" ");
        }

        printf("%c\n", rightChar);
    }

    //Display bottom delimiter
    printf(" ");
    for( counter = 0; counter < maxLength + 4 + lengthNumber; counter++ ) {
        printf("%c", bottomChar);
    }
    printf("\n");
}

/*
Goal : Count the number of figure in a number.
Input : number (short), number to process
Output : short, number of figure in number
*/
short countFigureInNumber(short number) {
    short counter = 0;

    while( number > 0 ) {
        number /= 10;
        counter++;
    }

    return counter;
}

/*
Goal : Clear the last '\n' char in a string
Input : str (char*), string to clear
Output : void
*/
void clearInput(char *str) {
    size_t length = strlen(str);
    size_t i = length - 1;

    while (i >= 0 && str[i] == '\n') {
        str[i--] = '\0';
    }
}

/*
Goal : Save the input from stdin into a variable
Input : - length (short), max length of the input
        - input (char*), variable in which we save the input
Output : void
*/
void getInput(short length, char *input) {
    flush();
    fgets(input, (int)length, stdin);
    clearInput(input);
}

/*
 * Goal : Flush the stdin buffer
 * Input : void
 * Output : void
 */
void flush() {
    int c;
    while ((c = getchar ()) != '\n'  &&  c != EOF){
        continue;
    }
}

/*
Goal : Returns the position of the last occurence of symbol in str
Input : - str (char*), string we want to search into
        - symbol (char), symbol we want to find in str
Output : char*, pointer to the last occurence of symbol in str.
*/
char* getLastOccInStr(char *str, char symbol) {
    char *temp = NULL;

    while( (str = strchr(str, symbol)) != NULL ) {
        temp = str;
        str++;
    }
    return temp;
}

/*
Goal : concatenate several str into one
Input : - resSize (short), size of res
        - res (char**), str to fill
        - strings (char**), array of str we want to concatenate
        - length (short), length of strings
Output : short, state of the process
            - 0, success
            - 1, strings is empty
            - 2, the sum of the length of strings is greater than resSize
*/
short concatenateSeveralStr(short resSize, char res[resSize], short length, char **strings) {
    if( length < 1 ) { //Le tableau de string est vide
        return 1;
    }

    short counter;
    int strLength = 0;
    char *temp;

    temp = res;

    //Compte la taille de la chaine de fin
    for( counter = 0; counter < length; counter++ ) {
        strLength += strlen(strings[counter]);
    }

    if( resSize < strLength ) { //Si la chaine de r�sultat n'est pas assez grande
        return 2;
    }

    //Concatene les chaines
    strcpy( temp, strings[0] );
    for( counter = 1; counter < length; counter++ ) {
        strcat(temp, strings[counter]);
    }

    res = temp;
    return 0;
}

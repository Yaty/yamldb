/*
**  Filename : general.c
**
**  Made by : Vincent GUÉNIN ESGI - 3AL-1
**
**  Description : Contains varied easy functions
*/

/*================ INCLUDES ================*/
#include <stdio.h>
#include <string.h>
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
Goal : Display a menu
Input : - length (short), length of the array
        - menuStr (char**), Str to display in the menu
Output : void
*/
void displayMenu(short length, char **menuStr) {
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

    //Take the number of figure in length
    lengthNumber = countFigureInNumber(length);

    //Display top delimiter
    printf(" ");
    for( counter = 0; counter < maxLength + 4 + lengthNumber; counter++ ) {
        printf("%c", topChar);
    }
    printf("\n");

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
    short length = strlen(str);
    if( str[ length - 1 ] == '\n' ) {
        str[ length - 1 ] = '\0';
    }
}

/*
Goal : Save the input from stdin into a variable
Input : - length (short), max length of the input
        - input (char*), variable in which we save the input
Output : void
*/
void getInput(short length, char *input) {
    fflush(stdin);
    fgets(input, (int)length, stdin);

    clearInput(input);
}

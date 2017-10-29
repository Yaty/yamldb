/*
**  Filename : general.h
**
**  Made by : Vincent GUÉNIN ESGI - 3AL-1
**
**  Description : Contains varied easy functions' prototypes
*/
#ifndef GENERAL_H_INCLUDED
#define GENERAL_H_INCLUDED

/*================ STRUCT DECLARATION ================*/


/*================ PROTOTYPES ================*/
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
short sizeOfBiggestSlotOfStrArray(short length, char **array);

/*
Goal : Display a menu
Input : - length (short), length of the array
        - menuStr (char**), Str to display in the menu
Output : void
*/
void displayMenu(short length, char **menuStr);

/*
Goal : Count the number of figure in a number.
Input : number (short), number to process
Output : short, number of figure in number
*/
short countFigureInNumber(short number);

/*
Goal : Clear the last '\n' char in a string
Input : str (char*), string to clear
Output : void
*/
void clear_input(char *str);

/*
Goal : Save the input from stdin into a variable
Input : - length (short), max length of the input
        - input (char*), variable in which we save the input
Output : void
*/
void getInput(short length, char *input);

/*
Goal : Returns the position of the last occurence of symbol in str
Input : - str (char*), string we want to search into
        - symbol (char), symbol we want to find in str
Output : char*, pointer to the last occurence of symbol in str.
*/
char* getLastOccInStr(char *str, char symbol);

#endif // GENERAL_H_INCLUDED

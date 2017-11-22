/*
**  Filename : general.h
**
**  Made by : Vincent GU�NIN ESGI - 3AL-1
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
Goal : Display title's menu
Input : - maxLength (short), length of the longest str
        - title (char*), title to display
        - leftChar (char), char to display on the left
        - rightChar (char), char to display on the right
Output : void
*/
void displayMenuTitle(short maxLength, char *title, char leftChar, char rightChar);

/*
Goal : Display a menu
Input : - title (char*), Str with the title of the menu
        - length (short), length of the array
        - menuStr (char**), Str to display in the menu
Output : void
*/
void displayMenu(char *title, short length, char **menuStr);

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
 * Goal : Flush the stdin buffer
 * Input : void
 * Output : void
 */
void flush();

/*
Goal : Returns the position of the last occurence of symbol in str
Input : - str (char*), string we want to search into
        - symbol (char), symbol we want to find in str
Output : char*, pointer to the last occurence of symbol in str.
*/
char* getLastOccInStr(char *str, char symbol);

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
short concatenateSeveralStr(short resSize, char res[resSize], short length, char **strings);

#endif // GENERAL_H_INCLUDED

/*
**  Filename : file_manager.h
**
**  Made by : Vincent GUÉNIN ESGI - 3AL-1
**
**  Description : Contains the functions' prototypes for files using
*/

#ifndef FILE_MANAGER_H_INCLUDED
#define FILE_MANAGER_H_INCLUDED

/*================ STRUCT DECLARATION ================*/


/*================ PROTOTYPES ================*/
/*
Goal : Check if a file exist
Input : filename (char*), path of the file we want to check if exist
Output : char (1 byte) :
            - 0, file doesn't exist
            - 1, file exist
*/
char fileExist(char *filename);

/*
Goal : Create a file
Input : filename (char*), path of the file we want to create
Output : char (1 byte) :
            - 0, file already exist
            - 1, error while creating the file
            - 2, file as been successfully created
*/
char createFile(char *filename);



#endif // FILE_MANAGER_H_INCLUDED

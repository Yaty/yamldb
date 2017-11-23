/*
**  Filename : file_manager.h
**
**  Made by : Vincent GU�NIN ESGI - 3AL-1
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

/*
Goal : Delete a file
Input : filename (char*), path to the file to delete
Output : short, state of the process
            - 0, success
            - 1, file does not exist
            - 2, error while deleting the file
*/
short deleteFile(char* filename);

/*
Goal : file is empty or nor
Input : path (char*), path to the file
Output : short, state of the process
            - 1, empty
            - 0, not empty
*/
int fileIsEmpty(char * path);

/**
 * Check if a folder exists or not
 * @param path the folder path
 * @return 1 if it exists, 0 otherwise
 */
int isFolderExists(char *path);

#endif // FILE_MANAGER_H_INCLUDED

/*
**  Filename : directory_functions.c
**
**  Made by : Vincent GU�NIN ESGI - 3AL-1
**
**  Description : Contains directory manipulation related functions
*/
#ifndef DIRECTORY_FUNCTIONS_H_INCLUDED
#define DIRECTORY_FUNCTIONS_H_INCLUDED

/*================ STRUCT DECLARATION ================*/


/*================ PROTOTYPES ================*/
/*
Goal : Returns the content of a directory
Input : - incomeArrayLength (short*), length of incomeArray (char***)
        - incomeArray (char***), array to fill. At the end of the function, it
            contains the list of directory and files contained in th directory
            pointed by path
        - path (char*), path of the directory we want to get content
Output : char, state of the function :
            - 0, success
            - 1, error while opening the directory
            - 2, error while memory allocation
Require : - income_array (char***) needs to be free.
          - income_array (char***) needs to be initialized - malloc(0) works
*/
char getDirectoryContent(short *incomeArrayLength, char ***incomeArray, char *path);

/*
Goal : Returns the list of sub-directory into a directory given in parameter.
Input : - incomeArrayLength (short*), length of incomeArray (char***)
        - incomeArray (char***), array to fill.
        - path (char*), path of the directory we want to list.
Output : char, state of the function :
            - 0, success
            - 1, error while opening the directory
            - 2, error while memory allocation
Require : - income_array (char***) needs to be free.
          - income_array (char***) needs to be initialized - malloc(0) works
*/
char getDirInDirectory(short *incomeArrayLength, char ***incomeArray, char* path);

/*
Goal : Returns the list of files into a directory given in parameter.
Input : - incomeArrayLength (short*), length of incomeArray (char***)
        - incomeArray (char***), array to fill.
        - path (char*), path of the directory we want to list.
Output : char, state of the function :
            - 0, success
            - 1, error while opening the directory
            - 2, error while memory allocation
Require : - income_array (char***) needs to be free.
          - income_array (char***) needs to be initialized - malloc(0) works
*/
char getFilesInDirectory(short *incomeArrayLength, char ***incomeArray, char* path);

/*
Goal : Create a directory
Input : dirPath (char*), path of the directory to create
Output : (void)
*/
void createDir(char *dirPath);

/*
Goal : Check if a dir exist in an other dir
Input : - dirPath (char*), path of the directory
        - dirName (char*), name of the directory we want to check if exist
Output : short, state of the process
            - 0, success, subdirectory doesn't exist
            - 1, success, subdir exist
            - - 1, error while listing the sub-directories
*/
short dirExist(char *dirPath, char *dirName);

#endif // DIRECTORY_FUNCTIONS_H_INCLUDED

//Kathar Patcha ABdul Rahim | 110134610 - Assignment 1

//This program is used to count the list of directories and
//files and get the size of all the files in the directory
//It also used to copy and move the directories


//Macros
//Need to compile and run nftw without any issues
#define _XOPEN_SOURCE 500
//Just a macro for utilizing inside program
#define BUFFER_SIZE 1024


//necessary headers
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <ftw.h>
#include <fcntl.h>
#include <limits.h>

//GLobal variables

//To keep track on file count
int fileCount = 0;

//To keep track on directory count
int dirCount = 0;

//To keep adding the size of all files
long long int totalSize = 0;

//Variable for extensions to exclude if given by user
static char *extensionToExclude = NULL;

//Variable for the source path
static const char *sourcePath = NULL;

//Variable for the destination path
static const char *destinationPath = NULL;

//variable for the source path - mutable
static char *initialSourcePath = NULL;

//Flag for move
static int moveFlag = 0;



//This function count the files, directories and calculates the size of the all the files in the directory passed
static int countFiles(const char *fpath, const struct stat *sb, int typeFlag, struct FTW *ftwBuffer)
{
    //Validating whether the given path is a file and if so increaste the file count and add the size
    if (typeFlag == FTW_F)
    {
        fileCount++;
        totalSize += sb->st_size;
    }
    //If the path is a directory increase the directory count
    else if (typeFlag == FTW_D)
        dirCount++;
    return 0;
}



//This function creates the relative path between the two given path
void getRelativePath(const char *path1, const char *path2, char *result)
{
   //Traverse until both paths have same common directory and copies the remaining content
   //means the unique content to the result
   while (*path1 && *path2 && (*path1 == *path2)) {
        path1++;
        path2++;
    }
    if (*path1 == '/') {
        path1++;
    }

    //Copying the remaining part of path1 to result
    strcpy(result, path1);

    //printf(result);
}


//This function removes last slash if a path has
//Just to satisy a corner testcase
void removeLastSlashIfExists(char *path) {
    size_t len = strlen(path);
    if (len > 0 && path[len - 1] == '/') {
        path[len - 1] = '\0';
    }
}




//This function constructs the dynamic destination path to creating replica of source heirarchy
void constructPath(const char *sourcePath, const char *destinationPath, char *dest_path)
{

//Assigning the path and the destination path to new variables
//Since const variables are immutable/
//creating the variables to the respective path's length
char *src_path = malloc(strlen(sourcePath) + 1);
char *dest_dir = malloc(strlen(destinationPath) + 1);
//copying the original value to the variables created
strcpy(src_path, sourcePath);
strcpy(dest_dir, destinationPath);


//removing last slash if user gives it by mistake
//It is like an error handling the last slash is given or not
//The program will continue to work fine
removeLastSlashIfExists(src_path);
removeLastSlashIfExists(dest_dir);
removeLastSlashIfExists(initialSourcePath);

//Creating a variable to store the relative path between the given path and destination path
char relativePath[PATH_MAX];

//It will fetch the main directory name means the root directory name which is being copied
char *mainDir = strrchr(initialSourcePath, '/');

//Copying the destination path to the result means dest_path
//concatenating the main dir to it
strcpy(dest_path, dest_dir);
strcat(dest_path, mainDir);

//calling the function to get the relative path
getRelativePath(sourcePath, initialSourcePath, relativePath);

//if relative path is empty just skip and move ahead without this path
if(strlen(relativePath) != 0)
{
//if relative path is not empty appending it to the dest_path
strcat(dest_path, "/");
strcat(dest_path, relativePath);
}
}


//This function copies the content of the source file to the destination file
//If there is no file in the destination, it will create and do the above process
void copyFile(const char *srcPath, const char *destPath) {

    //Validating whether the extension to exclude is not null
    //and so skip this function
    if(extensionToExclude != NULL && strcmp(strrchr(srcPath, '.'), extensionToExclude) == 0)
        return;

    //Opening the source file
    int srcFile = open(srcPath, O_RDONLY);
    if (srcFile == -1) {
        perror("Failed to open source file");
        exit(EXIT_FAILURE);
    }

    //Getting source file permissions
    struct stat st;
    if (stat(srcPath, &st) == -1) {
        perror("Failed to get source file information");
        exit(EXIT_FAILURE);
    }
    mode_t mode = st.st_mode;

    //Opening the destination file for writing
    int destFile = open(destPath, O_WRONLY | O_CREAT | O_TRUNC, mode);
    if (destFile == -1) {
        perror("Failed to open destination file");
        exit(EXIT_FAILURE);
    }

    //Copying contents from source file to destination file
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;
    while ((bytesRead = read(srcFile, buffer, BUFFER_SIZE)) > 0) {
        ssize_t bytesWritten = write(destFile, buffer, bytesRead);
        if (bytesWritten == -1) {
            perror("Failed to write to destination file");
            exit(EXIT_FAILURE);
        }
    }

    // Close files
    close(srcFile);
    close(destFile);

    //printf("File copied successfully from %s to %s\n", srcPath, destPath);
}



//This function creates a directory for the given path
int createDirectory(const char *directory) {
    //Creating the directory with read, write, and execute permissions
    if (mkdir(directory, S_IRWXU | S_IRWXG | S_IRWXO) != 0) {
        perror("mkdir");
        return -1;
    }

   // printf("Directory '%s' created successfully.\n", directory);
    return 0;
}


//This function traverse through all the paths sent by the nftw
//It also checks whether the path given is file or directory and takes necessary actions
int traverse(const char *path, const struct stat *sb, int typeFlag, struct FTW *ftwBuffer)
{

//creating a new variable to store the dynamic path
//obtained by the source dir and dest dir combination
//which will be used as the path to create file or direcotry
char newDestPath[PATH_MAX];

//calculating a function to construct the dynamic path
constructPath(path, destinationPath, newDestPath);

//Validating whether the path given is a file path or directory path
if(typeFlag == FTW_F)
{
 //Copying the file from the source path to the destination path
 copyFile(path, newDestPath);
}
else if(typeFlag == FTW_D)
{
 //Creating directories in destination as a replica of the source root directory
 createDirectory(newDestPath);
}

return 0;
}




//Main function start of the program
int main(int argc, char *argv[])
{

//validating the number of arguments passed is of required or more
if(argc < 3 || argc > 5)
{
printf("Invalid number of arguments...");
return 0;
}

//Assigning the option and the source Path to variables
char *option = argv[1];
sourcePath = argv[2];

//Validating the option given and taking decision based on that
if(strcmp(option, "-nf") == 0 || strcmp(option, "-nd") == 0 || strcmp(option, "-sf") == 0)
{
 //Using nftw to walk throughout the source directory
 if(nftw(sourcePath, countFiles, 20, FTW_PHYS) == -1)
 {
  //Printing the error if some issue occurs through the walkthrough
  printf("Error in nftw");
  return 0;
 }

 //Printing the desired values as mentioned by the user via option
 if(strcmp(option, "-nf") == 0)
        printf("\nTotal number of files : %d\n\n", fileCount);
 else if(strcmp(option, "-nd") == 0)
        printf("\nTotal number of directories : %d\n\n", dirCount);
 else if(strcmp(option, "-sf") == 0)
        printf("\nTotal size: %lld bytes\n\n", totalSize);
}

//Validating the option using else if for copy and move
else if(strcmp(option, "-cpx") == 0 || strcmp(option, "-mv") == 0)
{

//Stroring the source path in a different variable again which can be edited for future use
//Assigning the value of the destination to a variable
initialSourcePath = argv[2];
destinationPath = argv[3];


//Validating whether there are 5 arguments if so assigning the last one as
//the extension that need to be excluded
if(argc == 5)
        extensionToExclude = argv[4];

//Validating whether the option given is move and if so turning on the move flag
if(strcmp(option, "-mv") == 0)
        moveFlag = 1;

//Using nftw to walk throughout the directory
if(nftw(sourcePath, traverse, 20, FTW_PHYS) == -1)
 {
  printf("Error in nftw");
  return 0;
 }

//Validating the option is copy and so printing the success message for it
if(strcmp(option, "-cpx") == 0)
{
printf("\nDirectory successfully copied...\n\n");
}

}

//Validating whether the move flag is on and so deleting the source directory
//So moving is like copying and then deleting the source directory
if(moveFlag != 0)
{
printf("\nPath going to be deleted: %s\n", sourcePath);

char command[1024];

    //Ensuring the command buffer is large enough to hold the command string
    //Also concatenating the rm -r command along with the source path
    snprintf(command, sizeof(command), "rm -r \"%s\"", sourcePath);

    // Executing the command by calling a system call
    int ret = system(command);

    //Validating whether the remove works fine or not
    if (ret == -1) {
        perror("Error executing system command");
    } else if (WIFEXITED(ret) && WEXITSTATUS(ret) != 0) {
        fprintf(stderr, "Command failed with exit status %d\n", WEXITSTATUS(ret));
    }
//printing the success message for the move operation
printf("\nDirectory suucessfully moved..,\n\n");
return 0;
}

return 0;
}

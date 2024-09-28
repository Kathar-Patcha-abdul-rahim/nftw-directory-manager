dftw C Program
Overview
The dftw C program performs multiple directory and file management operations using command-line arguments. By leveraging the nftw() system call, it allows for recursive traversal of directory trees, enabling actions like counting files, directories, copying/moving directories, and calculating file sizes.

Features
List File Count (-nf): Counts all files in a directory tree rooted at a specified directory.
List Directory Count (-nd): Counts all directories in a directory tree.
File Size Listing (-sf): Lists the size of all files in the directory tree.
Copy Directory (-cpx): Copies an entire directory tree, excluding files with specific extensions.
Move Directory (-mv): Moves an entire directory tree to a new location.
Usage
The program accepts various command-line arguments for different operations:

Count Files:

$ dftw -nf [root_dir]
Example:
dftw -nf /home/user/docs
Lists the number of files in /home/user/docs and all its subdirectories.

Count Directories:

$ dftw -nd [root_dir]
Example:
dftw -nd /home/user/docs
Lists the number of directories in /home/user/docs.

List File Sizes:

$ dftw -sf [root_dir]
Example:
dftw -sf /home/user/docs
Displays the size of all files in the directory tree rooted at /home/user/docs.

Copy Directory Excluding File Extensions:

$ dftw -cpx [source_dir] [destination_dir] [file_extension]
Example:
dftw -cpx /home/user/docs /home/user/backup .txt
Copies the /docs directory to /backup, excluding .txt files.

Move Directory:

$ dftw -mv [source_dir] [destination_dir]
Example:
dftw -mv /home/user/docs /home/user/backup
Moves the entire /docs directory to /backup.

Dependencies
Requires a UNIX-based system with support for the nftw() system call.
Developed in C with standard libraries.

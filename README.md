# dftw - Directory and File Tree Walker

## Overview

The **dftw** C program performs various directory and file management operations using command-line arguments. It leverages the `nftw()` system call for recursive traversal of directory trees, allowing actions such as counting files, counting directories, copying/moving directories, and calculating file sizes.

## Features

- **File Count (-nf)**: Counts all files in a directory tree rooted at a specified directory.
- **Directory Count (-nd)**: Counts all directories in a directory tree.
- **File Size Listing (-sf)**: Lists the sizes of all files in the directory tree.
- **Copy Directory (-cpx)**: Copies an entire directory tree, excluding files with specific extensions.
- **Move Directory (-mv)**: Moves an entire directory tree to a new location.

## Usage

Open the attached pdf file for learn about more usages

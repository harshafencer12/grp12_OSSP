# File Permissions Using Unix Commands

## Objective

To create a text file using a C program and demonstrate how to view and modify file permissions using Unix commands and the `chmod` command.

## Program Description

A C program is used to create a text file named `sample.txt`. The program uses `fopen()` to create the file in write mode and `fprintf()` to write text into the file.

## Source Code

The program uses the following C functions:

- `fopen()` - Opens or creates the file.
- `fprintf()` - Writes data into the file.
- `fclose()` - Closes the file.

## Compilation

```bash
gcc file_permission.c -o file_permission

#ifndef APP_FILE_H_
#define APP_FILE_H_

#include <dirent.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

struct dirent* myReaddir(DIR* x)
{
    struct dirent* directory;
    directory = readdir(x);
    if (directory != NULL && (strcmp(directory->d_name, ".") == 0 || strcmp(directory->d_name, "..") == 0)) {
        return myReaddir(x);
    }
    return directory;
}

void nextFile(char* filename, const char* folder, const char* current, int8_t direction = 0)
{
    struct dirent* directory;
    DIR* x = opendir(folder);

    char* previous = NULL;

    if (x != NULL) {
        while ((directory = myReaddir(x)) != NULL) {
            if (direction == 0) {
                strncpy(filename, directory->d_name, 256);
                return;
            }
            if (strcmp(current, directory->d_name) == 0) {
                strncpy(filename, directory->d_name, 256);
                if (direction < 0) {
                    if (previous != NULL) {
                        strncpy(filename, previous, 256);
                    }
                    return;
                }
                if ((directory = myReaddir(x)) != NULL) {
                    strncpy(filename, directory->d_name, 256);
                    return;
                }
                return;
            }
            previous = directory->d_name;
        }
        closedir(x);
    }

    strncpy(filename, "Empty folder", 256);
}

#endif
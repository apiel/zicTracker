#ifndef APP_FILE_H_
#define APP_FILE_H_

#include <stdio.h>
#include <stdint.h>
#include <dirent.h>
#include <string.h>

void nextFile(char* filename, const char* folder, const char* current, int8_t direction = 1)
{
    struct dirent* directory; // creating pointer of type dirent
    DIR* x = opendir(folder); // it will open directory

    char* previous = (char *)"Empty folder";

    if (x != NULL) {
        while ((directory = readdir(x)) != NULL) {
            if (strcmp(directory->d_name, ".") == 0 || strcmp(directory->d_name, "..") == 0) {
                continue;
            }
            if (strcmp(current, directory->d_name) == 0) {
                if (direction < 0) {
                    break;
                }
                previous = directory->d_name;
                if ((directory = readdir(x)) != NULL) {
                    snprintf(filename, 256, "%s", directory->d_name);
                    return;
                }
            }
            previous = directory->d_name;
        }
        closedir(x);
    }

    snprintf(filename, 256, "%s", previous);
}

#endif
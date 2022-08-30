#ifndef APP_FILE_H_
#define APP_FILE_H_

#include <dirent.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <sys/stat.h> // mkdir
#include <sys/types.h> // mkdir

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

enum {
    FILE_NONE,
    FILE_SUCCESS,
};

#define MAX_FILENAME 100

template <typename... Args>
uint8_t loadFileContent(char* content, uint16_t len, const char* fmt, Args... args)
{
    char filename[MAX_FILENAME];
    snprintf(filename, MAX_FILENAME, fmt, args...);

    Zic_File file(filename, "r");
    if (file.isOpen()) {
        file.read(content, len);
        file.close();
        return FILE_SUCCESS;
    }

    return FILE_NONE;
}

uint8_t loadFileContent(char* content, uint16_t len, const char* fmt)
{
    return loadFileContent(content, len, fmt, NULL);
}

template <typename... Args>
uint8_t saveFileContent(const char* mode, char* content, uint16_t len, const char* fmt, Args... args)
{
    char filename[MAX_FILENAME];

    snprintf(filename, MAX_FILENAME, fmt, args...);
    strrchr(filename, '/')[0] = '\0';
    mkdir(filename, 0777);

    snprintf(filename, MAX_FILENAME, fmt, args...);
    Zic_File file(filename, mode);
    if (file.isOpen()) {
        file.write(content, len);
        file.close();
        return FILE_SUCCESS;
    }
    return FILE_NONE;
}

template <typename... Args>
uint8_t saveFileContent(char* content, uint16_t len, const char* fmt, Args... args)
{
    return saveFileContent("w", content, len, fmt, args...);
}

uint8_t saveFileContent(char* content, uint16_t len, const char* fmt)
{
    return saveFileContent(content, len, fmt, NULL);
}

uint8_t saveFileContent(const char* mode, char* content, uint16_t len, const char* fmt)
{
    return saveFileContent(mode, content, len, fmt, NULL);
}

#endif
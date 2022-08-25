#ifndef APP_FILE_PROJECT_H_
#define APP_FILE_PROJECT_H_

#include <stdio.h>
#include <stdint.h>

#include "./app_file.h"

#define PROJECT_FILE_FORMAT "projects/%d/project.config", project

uint8_t loadFileProject(uint8_t project, char* content, uint16_t len)
{
    return loadFileContent(content, len, PROJECT_FILE_FORMAT);
}

uint8_t saveFileProject(uint8_t project, char* content, uint16_t len)
{
    return saveFileContent(content, len, PROJECT_FILE_FORMAT);
}

#endif
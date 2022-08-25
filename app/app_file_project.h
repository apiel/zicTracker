#ifndef APP_FILE_PROJECT_H_
#define APP_FILE_PROJECT_H_

#include <stdio.h>
#include <stdint.h>

#include "./app_file.h"

#define MAX_PATTERN_FILENAME 100
char projectFilepath[MAX_PATTERN_FILENAME];

void setProjectFilename(uint8_t project)
{
    snprintf(projectFilepath, MAX_PATTERN_FILENAME, "projects/%d/project.config", project);
}

uint8_t loadFileProject(uint8_t project, uint8_t pos, char* content, uint16_t len)
{
    setProjectFilename(project);
    return loadFileContent(projectFilepath, content, len);
}

uint8_t saveFileProject(uint8_t project, uint8_t pos, char* content, uint16_t len)
{
    setProjectFilename(project);
    return saveFileContent(projectFilepath, content, len);
}

#endif
#ifndef APP_FILE_PATTERN_H_
#define APP_FILE_PATTERN_H_

#include <stdio.h>
#include <stdint.h>

#include "./app_file.h"

#define MAX_PATTERN_FILENAME 100
char patternFilepath[MAX_PATTERN_FILENAME];

void setPatternFilename(uint8_t project, uint8_t pos)
{
    snprintf(patternFilepath, MAX_PATTERN_FILENAME, "projects/%d/patterns/%02X.pat", project, pos);
}

uint8_t loadFilePattern(uint8_t project, uint8_t pos, char* content, uint16_t len)
{
    setPatternFilename(project, pos);
    return loadFileContent(patternFilepath, content, len);
}

uint8_t saveFilePattern(uint8_t project, uint8_t pos, char* content, uint16_t len)
{
    setPatternFilename(project, pos);
    return saveFileContent(patternFilepath, content, len);
}

#endif
#ifndef APP_FILE_PATTERN_H_
#define APP_FILE_PATTERN_H_

#include <stdint.h>
#include <stdio.h>

#include "./app_file.h"

#define PATTERN_FILE_FORMAT "projects/%d/patterns/%02X.pat", project, pos

uint8_t loadFilePattern(uint8_t project, uint8_t pos, char* content, uint16_t len)
{
    return loadFileContent(content, len, PATTERN_FILE_FORMAT);
}

uint8_t saveFilePattern(uint8_t project, uint8_t pos, char* content, uint16_t len)
{
    return saveFileContent(content, len, PATTERN_FILE_FORMAT);
}

#endif
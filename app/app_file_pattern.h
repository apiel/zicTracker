#ifndef APP_FILE_PATTERN_H_
#define APP_FILE_PATTERN_H_

#include <stdio.h>
#include <stdint.h>

#include <zic_file.h>

enum {
    PAT_LOAD_NONE,
    PAT_LOAD_SUCCESS,
};

enum {
    PAT_SAVE_NONE,
    PAT_SAVE_SUCCESS,
};

#define MAX_PATTERN_FILENAME 100
char patternFilepath[MAX_PATTERN_FILENAME];

void setPatternFilename(uint8_t project, uint8_t pos)
{
    snprintf(patternFilepath, MAX_PATTERN_FILENAME, "projects/%d/patterns/%d.pat", project, pos);
}

uint8_t loadFilePattern(uint8_t project, uint8_t pos, char* content, uint16_t len)
{
    setPatternFilename(project, pos);
    Zic_File file(patternFilepath, "r");
    if (file.isOpen()) {
        file.read(content, len);
        file.close();
        return PAT_LOAD_SUCCESS;
    }
    return PAT_LOAD_NONE;
}

uint8_t saveFilePattern(uint8_t project, uint8_t pos, char* content, uint16_t len)
{
    setPatternFilename(project, pos);
    Zic_File file(patternFilepath, "w");
    if (file.isOpen()) {
        file.seekFromStart(0);
        file.write(content, len);
        file.close();
        return PAT_SAVE_SUCCESS;
    }
    return PAT_SAVE_NONE;
}
#endif
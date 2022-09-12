#ifndef APP_UTIL_H_
#define APP_UTIL_H_

#include <string.h>

const char* alphanum = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

const char* charLevel(uint8_t level)
{
    switch (level) {
    case 0:
        return " ";
    case 1:
        return "⠠";
    case 2:
        return "⠤";
    case 3:
        return "⠴";
    case 4:
        return "⠶";
    case 5:
        return "⠾";
    default:
        return "⠿";
    }
}

uint16_t levelSpecialChar[6] = {
    *(uint16_t*)("⠠" + 1),
    *(uint16_t*)("⠤" + 1),
    *(uint16_t*)("⠴" + 1),
    *(uint16_t*)("⠶" + 1),
    *(uint16_t*)("⠾" + 1),
    *(uint16_t*)("⠿" + 1),
};

uint8_t getLevel(uint16_t val)
{
    uint8_t level = 0;
    for (; level < 6; level++) {
        if (val == levelSpecialChar[level]) {
            break;
        }
    }
    return level;
}

uint8_t alphanumToInt(char c)
{
    // for (uint8_t i = 0; i < sizeof(alphanum); i++) {
    //     if (alphanum[i] == c) {
    //         return i;
    //     }
    // }
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'A' && c <= 'Z')
        return c - 'A' + 10;
    return 0;
}

void trimToNumeric(char* str)
{
    char* p = str;
    while (*p) {
        if (*p < '0' || *p > '9') {
            *p = 0;
            break;
        }
        p++;
    }
}

void removeLeadingSpaces(char* str)
{
    char* p = str;
    while (*p) {
        if (*p != ' ') {
            break;
        }
        p++;
    }
    if (p != str) {
        strcpy(str, p);
    }
}

#endif
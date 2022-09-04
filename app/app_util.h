#ifndef APP_UTIL_H_
#define APP_UTIL_H_

#include <string.h>

const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

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
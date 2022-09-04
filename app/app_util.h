#ifndef APP_UTIL_H_
#define APP_UTIL_H_

#include <string.h>

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
#ifndef APP_DISPLAY_H_
#define APP_DISPLAY_H_

#include <stdint.h>
// #include <string.h>
#include <cstring>

#define MAX_DISPLAY_TEXT 176 // 21*8 + 8\n
                             // but should actually be 20*8 to fit console

class App_Display {
public:
    char text[MAX_DISPLAY_TEXT] = "";
    char* cursorPos = NULL;
    uint8_t cursorLen = 0;

    void setCursor(uint8_t len, int8_t start = 0)
    {
        cursorPos = text + strlen(text) + start;
        cursorLen = len;
    }
};

#endif

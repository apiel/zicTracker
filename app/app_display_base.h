#ifndef APP_DISPLAY_BASE_H_
#define APP_DISPLAY_BASE_H_

#include <stdint.h>
#include <cstring>

// #define MAX_DISPLAY_TEXT 176 // 21*8 + 8\n
//                              // but should actually be 20*8 to fit console

#define MAX_DISPLAY_TEXT 512

class App_Display_Base {
public:
    char text[MAX_DISPLAY_TEXT] = "";
    char* cursorPos = NULL;
    uint8_t cursorLen = 0;
    uint8_t coloredLabel = 255;
    uint8_t coloredLabelFrom = 0;
    int8_t coloredHeader[2] = { -1, -1 };

    uint8_t startRow = 0;

    virtual void reset()
    {
        cursorPos = NULL;
        cursorLen = 0;
        coloredLabel = 255;
        coloredLabelFrom = 0;
        useColoredHeader(-1, -1);
    }

    virtual bool ready()
    {
        return true;
    }

    void setCursor(uint8_t len, int8_t start = 0)
    {
        cursorPos = text + strlen(text) + start;
        cursorLen = len;
    }

    void useColoredLabel(uint8_t pos = 0, uint8_t from = 0)
    {
        coloredLabel = pos;
        coloredLabelFrom = from;
    }

    bool isColoredLabel()
    {
        return coloredLabel != 255;
    }

    void useColoredHeader()
    {
        useColoredHeader(0, -1);
    }

    void useColoredHeader(int8_t row)
    {
        useColoredHeader(row, -1);
    }

    void useColoredHeader(int8_t row1, int8_t row2)
    {
        coloredHeader[0] = row1;
        coloredHeader[1] = row2;
    }
};

#endif

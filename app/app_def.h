#ifndef APP_DEF_H_
#define APP_DEF_H_

#include <stdint.h>

enum{
    VIEW_NONE,
    VIEW_DONE,
};

enum
{
    TRACK_1,
    TRACK_2,
    TRACK_3,
    TRACK_4,
    TRACK_COUNT
};

enum
{
    UI_KEY_UP,
    UI_KEY_DOWN,
    UI_KEY_LEFT,
    UI_KEY_RIGHT,
    UI_KEY_A,
    UI_KEY_B,
    UI_KEY_COUNT
};

typedef struct {
    bool Up = false;
    bool Down = false;
    bool Right = false;
    bool Left = false;
    bool A = false;
    bool B = false;
} UiKeys;

#endif
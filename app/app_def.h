#ifndef APP_DEF_H_
#define APP_DEF_H_

#include <stdint.h>

#ifndef PATTERN_COUNT
#define PATTERN_COUNT 200
#endif

enum {
    VIEW_NONE,
    VIEW_CHANGED,
};

enum {
    VIEW_MENU,
    VIEW_TRACK,
    VIEW_TRACK_DELAY,
    VIEW_TRACK_SEQUENCER,
    VIEW_PATTERN,
    VIEW_INSTRUMENT,
    VIEW_SAMPLER,
    VIEW_PROJECT,
    VIEW_COUNT
};

enum {
    TRACK_1,
    TRACK_2,
    TRACK_3,
    TRACK_4,
    TRACK_COUNT
};

enum {
    INSTRUMENT_1,
    INSTRUMENT_2,
    INSTRUMENT_3,
    INSTRUMENT_4,
    INSTRUMENT_COUNT
};

enum {
    DELAY_1,
    DELAY_2,
    DELAY_3,
    DELAY_4,
    DELAY_5,
    DELAY_COUNT
};

enum {
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
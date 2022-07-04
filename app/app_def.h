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
    VIEW_TRACK,
    VIEW_TRACK_LOOP,
    VIEW_PATTERN,
    VIEW_INSTRUMENT,
    VIEW_WAVETABLE_SAMPLER,
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
    INSTRUMENT_5,
    INSTRUMENT_6,
    INSTRUMENT_7,
    INSTRUMENT_8,
    INSTRUMENT_9,
    INSTRUMENT_10,
    INSTRUMENT_11,
    INSTRUMENT_12,
    INSTRUMENT_13,
    INSTRUMENT_14,
    INSTRUMENT_15,
    INSTRUMENT_16,
    INSTRUMENT_COUNT
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
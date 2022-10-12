#ifndef APP_DEF_H_
#define APP_DEF_H_

#include <stdint.h>

#include <app_core_def.h>

#define PATTERN_COUNT 255 // uint8 -> 2 hex

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
    EVENT_VIEW_ON_TEMPO,
};

enum {
    DELAY_1,
    DELAY_2,
    DELAY_3,
    DELAY_4,
    DELAY_5,
    DELAY_COUNT
};

#endif
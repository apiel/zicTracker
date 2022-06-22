#ifndef APP_PATTERNS_H_
#define APP_PATTERNS_H_

#include <stdio.h>
#include <string.h>
#include <zic_note.h>
#include <zic_seq_pattern.h>

#include "./app_def.h"

#define STEP_DATA_LEN 8
#define PATTERN_DATA_LEN 8 * MAX_STEPS_IN_PATTERN // 8*64=512
#define SAME_INSTRUMENT_SYMBOL '_'

enum {
    PAT_LOAD_NONE,
    PAT_LOAD_SUCCESS,
};

class App_Patterns {
protected:
    uint8_t project = 0;
    char data[PATTERN_DATA_LEN];
    uint8_t (*loadFn)(uint8_t project, uint8_t pos, char* content);
    void (*saveFn)(uint8_t project, uint8_t pos, char* content);

public:
    Zic_Seq_Pattern patterns[PATTERN_COUNT];

    App_Patterns(
        uint8_t (*_loadFn)(uint8_t project, uint8_t pos, char* content),
        void (*_saveFn)(uint8_t project, uint8_t pos, char* content))
        : loadFn(_loadFn)
        , saveFn(_saveFn)
    {
        // NOTE instead to load all pattern in memory, could also load on demand
        for (uint8_t p = 0; p < PATTERN_COUNT; p++) {
            load(p);
        }
    }

    void load(uint8_t pos)
    {
        if (loadFn(project, pos + 1, data)) {
            Zic_Seq_Step* step = patterns[pos].steps;
            uint8_t prevInstrument = 255;
            uint8_t count = 0;
            for (uint16_t d = 0; d < PATTERN_DATA_LEN; d += STEP_DATA_LEN, step++, count++) {
                char* stepData = data + d;
                if (stepData[0] != SAME_INSTRUMENT_SYMBOL && (stepData[0] < 'A' || stepData[0] > 'Z')) {
                    break;
                }
                step->instrument = stepData[0] == SAME_INSTRUMENT_SYMBOL ? prevInstrument : 'A' - stepData[0];
                prevInstrument = step->instrument;
                step->note = stepData[2] == '-' ? 0 : charNotetoInt(stepData[2], stepData[3], stepData[4]);
                step->slide = stepData[6] == '1';
            }
            patterns[pos].stepCount = count;
        }
    }

    void save(uint8_t pos)
    {
        Zic_Seq_Pattern* pattern = &patterns[pos];
        uint8_t prevInstrument = 255;
        for (uint8_t s = 0; s < pattern->stepCount; s++) {
            Zic_Seq_Step* step = &pattern->steps[s];
            snprintf(data + strlen(data), STEP_DATA_LEN, "%c %s%d %d\n", // 8 char "A,C 4,1;"
                prevInstrument == step->instrument ? SAME_INSTRUMENT_SYMBOL : step->instrument + 'A',
                getNoteStr(step->note), getNoteOctave(step->note), step->slide);
        }
        saveFn(project, pos + 1, data);
    }

    void debug(void (*log)(const char* fmt, ...))
    {
        for (uint8_t pos = 0; pos < PATTERN_COUNT; pos++) {
            debug(log, pos + 1);
        }
    }

    void debug(void (*log)(const char* fmt, ...), uint8_t pos)
    {
        pos--;
        log("Pattern %d (%d steps):", pos + 1, patterns[pos].stepCount);
        for (uint8_t s = 0; s < patterns[pos].stepCount; s++) {
            Zic_Seq_Step* step = &patterns[pos].steps[s];
            if (step->note) {
                log(" [%d,%s%d%s]", step->instrument,
                    getNoteStr(step->note), getNoteOctave(step->note), step->slide ? ",slide" : "");
            } else {
                log(" [%d,---%s]", step->instrument, step->slide ? ",slide" : "");
            }
        }
        log("\n");
    }
};

#endif
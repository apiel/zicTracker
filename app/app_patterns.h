#ifndef APP_PATTERNS_H_
#define APP_PATTERNS_H_

#include <stdio.h>
#include <string.h>
#include <zic_note.h>
#include <zic_seq_pattern.h>

#include "./app_def.h"
#include "./app_file_pattern.h"

#define STEP_DATA_LEN 9
#define PATTERN_DATA_LEN STEP_DATA_LEN * MAX_STEPS_IN_PATTERN
#define SAME_INSTRUMENT_SYMBOL '_'

class App_Patterns {
protected:
    uint8_t project = 0;
    char data[PATTERN_DATA_LEN];

public:
    Zic_Seq_Pattern patterns[PATTERN_COUNT];

    App_Patterns()
    {
        // NOTE instead to load all pattern in memory, could also load on demand
        for (uint8_t p = 0; p < PATTERN_COUNT; p++) {
            load(p);
        }
    }

    void load(uint8_t pos)
    {
        // clear data
        for (uint16_t i = 0; i < PATTERN_DATA_LEN; i++) {
            data[i] = '\0';
        }
        if (loadFilePattern(project, pos + 1, data, PATTERN_DATA_LEN)) {
            Zic_Seq_Step* step = patterns[pos].steps;
            uint8_t prevInstrument = 255;
            uint8_t count = 0;
            for (uint16_t d = 0; d < PATTERN_DATA_LEN; d += STEP_DATA_LEN - 1, step++, count++) {
                char* stepData = data + d;
                if (stepData[0] != SAME_INSTRUMENT_SYMBOL && (stepData[0] < 'A' || stepData[0] > 'Z')) {
                    // printf("exit loop %d at step %d\n", pos + 1, count);
                    break;
                }
                step->instrument = stepData[0] == SAME_INSTRUMENT_SYMBOL ? prevInstrument : (stepData[0] - 'A');
                // printf("Instrument %c val %d\n", stepData[0], step->instrument);
                prevInstrument = step->instrument;
                step->note = stepData[2] == '-' ? 0 : Zic::charNotetoInt(stepData[2], stepData[3], stepData[4]);
                step->slide = stepData[6] == '1';
            }
            patterns[pos].stepCount = count;
        }
        patterns[pos].id = pos;
    }

    void save(uint8_t pos)
    {
        Zic_Seq_Pattern* pattern = &patterns[pos];
        uint8_t prevInstrument = 255;
        for (uint8_t s = 0; s < pattern->stepCount; s++) {
            Zic_Seq_Step* step = &pattern->steps[s];
            char instrument = prevInstrument == step->instrument ? SAME_INSTRUMENT_SYMBOL : step->instrument + 'A';
            prevInstrument = step->instrument;
            if (step->note) {
                snprintf(data + strlen(data), STEP_DATA_LEN, "%c %s%d %c\n", // 8 char "A C-4 1\n"
                    instrument, Zic::getNoteDash(step->note), Zic::getNoteOctave(step->note), step->slide ? '1' : '0');
            } else {
                // 8 char "A --- 0\n"
                snprintf(data + strlen(data), STEP_DATA_LEN, "%c --- %c\n", instrument, step->slide ? '1' : '0');
            }
        }
        saveFilePattern(project, pos + 1, data, strlen(data));
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
        log("[%d] Pattern %d (%d steps):", patterns[pos].id, pos + 1, patterns[pos].stepCount);
        for (uint8_t s = 0; s < patterns[pos].stepCount; s++) {
            Zic_Seq_Step* step = &patterns[pos].steps[s];
            if (step->note) {
                log(" [%d,%s%d (%d)%s]", step->instrument,
                    Zic::getNoteDash(step->note), Zic::getNoteOctave(step->note), step->note, step->slide ? ",slide" : "");
            } else {
                log(" [%d,--- (%d)%s]", step->instrument, step->note, step->slide ? ",slide" : "");
            }
        }
        log("\n");
    }
};

#endif
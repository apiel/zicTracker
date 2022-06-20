#ifndef APP_PATTERNS_H_
#define APP_PATTERNS_H_

#include <stdio.h>
#include <string.h>
#include <zic_note.h>
#include <zic_seq_pattern.h>

#include "./app_def.h"

#define STEP_DATA_LEN 8
#define PATTERN_DATA_LEN 8 * MAX_STEPS_IN_PATTERN // 8*64=512

class App_Patterns {
protected:
    char name[3];
    char data[PATTERN_DATA_LEN];
    void (*loadFn)(uint8_t project, char* name, char* content);
    void (*saveFn)(uint8_t project, char* name, char* content);

    void setName(uint8_t bank, uint8_t pos)
    {
        snprintf(name, 3, "%c%d", 'A' + bank, pos);
    }

public:
    Zic_Seq_Pattern patterns[BANK_COUNT][PATTERN_PER_BANK];

    App_Patterns(
        void (*_loadFn)(uint8_t project, char* name, char* content),
        void (*_saveFn)(uint8_t project, char* name, char* content))
        : loadFn(_loadFn)
        , saveFn(_saveFn)
    {
    }

    void load(uint8_t project, uint8_t bank, uint8_t pos)
    {
        setName(bank, pos);
        loadFn(project, name, data);
        Zic_Seq_Step* step = patterns[bank][pos].steps;
        for (uint16_t d = 0; d < PATTERN_DATA_LEN; d += STEP_DATA_LEN, step++) {
            // TODO end?
            char* stepData = data + d;
            step->instrument = 'A' - stepData[0];
            step->note = charNotetoInt(stepData[2], stepData[3], stepData[4]);
            step->slide = stepData[7] == '1';
        }
    }

    void save(uint8_t project, uint8_t bank, uint8_t pos)
    {
        setName(bank, pos);
        Zic_Seq_Pattern* pattern = &patterns[bank][pos];
        uint8_t prevInstrument = 255;
        for (uint8_t s = 0; s < pattern->stepCount; s++) {
            Zic_Seq_Step* step = &pattern->steps[s];
            snprintf(data + strlen(data), STEP_DATA_LEN, "%c,%s%d,%d;", // 8 char "A,C 4,1;"
                prevInstrument == step->instrument ? '_' : step->instrument + 'A',
                getNoteStr(step->note), getNoteOctave(step->note), step->slide);
        }
        saveFn(project, name, data);
    }

    void debug(void* log(const char* fmt, ...))
    {
        for (uint8_t b = 0; b < BANK_COUNT; b++) {
            for (uint8_t p = 0; p < PATTERN_PER_BANK; p++) {
                log("%c%d:", 'A' + b, p);
                for (uint8_t s = 0; s < patterns[b][p].stepCount; s++) {
                    Zic_Seq_Step* step = &patterns[b][p].steps[s];
                    log(" [%d,%s%d%s]", step->instrument,
                        getNoteStr(step->note), getNoteOctave(step->note), step->slide ? ",slide" : "");
                }
                log("\n");
            }
        }
    }
};

#endif
#ifndef APP_STATE_PATTERN_H_
#define APP_STATE_PATTERN_H_

#include <app_core_file.h>
#include <stdint.h>
#include <zic_seq_pattern.h>

// FIXME duplicated
#define VELOCITY_ARR uint8_t velocity[6] = { 50, 70, 85, 100, 110, 127 };

class App_State_Pattern {
protected:
    // FIXME duplicated
    VELOCITY_ARR

    // FIXME duplicated
    uint8_t getVel(Zic_Seq_Step* step)
    {
        uint8_t vel = 0;
        for (; vel < 6; vel++) {
            if (step->velocity <= velocity[vel]) {
                return vel;
            }
        }
        return vel;
    }

public:
    Zic_Seq_Pattern patterns[PATTERN_COUNT];

    const char* statePath = "projects/current/patterns/pattern%03d_%02X.zic";

    void save()
    {
        APP_LOG("Save state %s\n", statePath);

        for (uint8_t id = 0; id < PATTERN_COUNT; id++) {
            char filepath[100];
            sprintf(filepath, statePath, id + 1, id + 1);

            Zic_Seq_Pattern* pattern = &patterns[id];
            Zic_File file(filepath, "w");
            for (uint8_t s = 0; s < pattern->stepCount; s++) {
                char text[64] = "";
                uint8_t len = 0;
                for (uint8_t v = 0; v < MAX_VOICES_IN_PATTERN; v++) {
                    Zic_Seq_Step* step = &pattern->steps[v][s];
                    if (step->note == 0) {
                        len += sprintf(text + strlen(text), "---");
                    } else {
                        len += sprintf(text + strlen(text), "%2s%d", Zic::getNoteUnderscore(step->note), Zic::getNoteOctave(step->note));
                    }
                    char condition[3];
                    step->getConditionName(condition);
                    len += sprintf(text + strlen(text), "%s%s%s", charLevel(getVel(step) + 1), condition, step->slide ? "⤸" : " ");
                }
                len += sprintf(text + strlen(text), "\n");
                file.write(text, len);
            }
            file.close();
        }
    }

    void load()
    {
        APP_LOG("Load state %s\n", statePath);

        for (uint8_t id = 0; id < PATTERN_COUNT; id++) {
            char filepath[100];
            sprintf(filepath, statePath, id + 1, id + 1);

            Zic_File file(filepath, "r");
            if (file.isOpen()) {
                Zic_Seq_Pattern* pattern = &patterns[id];
                pattern->id = id;
                char stepStr[11];
                uint8_t s = 0;
                for (; s < MAX_STEPS_IN_PATTERN; s++) {
                    for (uint8_t i = 0; i < INSTRUMENT_COUNT; i++) {
                        if (!file.read(stepStr, 11)) {
                            goto exitloop;
                        }
                        Zic_Seq_Step* step = &pattern->steps[i][s];
                        char condition[3];
                        memcpy(condition, stepStr + 6, 2);
                        condition[2] = 0;
                        step->setCondition(condition);
                        step->slide = stepStr[9] == -92;
                        step->velocity = velocity[getLevel(*(uint16_t*)(stepStr + 4))];
                        step->note = stepStr[0] == '-' ? 0 : Zic::charNotetoInt(stepStr[0], stepStr[1], stepStr[2]);
                    }
                    file.seekFromCurrent(1);
                }
                exitloop:
                printf("Loaded %d steps in pattern %d\n", s, id);
                pattern->stepCount = s;
                file.close();
            }
        }
    }
};

#endif
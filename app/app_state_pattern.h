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

    const char* statePath = "projects/current/patterns/pattern%03d_%02X_.zic";

    void save()
    {
        APP_LOG("Save state %s\n", statePath);

        for (uint8_t id = 0; id < PATTERN_COUNT; id++) {
            // renderer->startRow = 0;
            // setLastRow(App_State::getInstance()->patterns[currentPatternId].stepCount + VIEW_PATTERN_ROW_HEADERS);
            // for (uint8_t row = 0; row < lastRow; row += TABLE_VISIBLE_ROWS, renderer->startRow += TABLE_VISIBLE_ROWS) {
            //     render(renderer);
            // saveFileContent(row == 0 ? "w" : "a", renderer->text, strlen(renderer->text),
            //     snapshotPath, currentPatternId + 1, currentPatternId + 1);
            // }

            // sprintf(text + strlen(text), statePath, id + 1, id + 1);

            char filepath[100];
            sprintf(filepath, statePath, id + 1, id + 1);

            Zic_Seq_Pattern* pattern = &patterns[id];
            // printf("Save pattern (%d) %s\n", pattern->stepCount, filepath);
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
    }
};

#endif
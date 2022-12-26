#ifndef APP_STATE_TRACK_H_
#define APP_STATE_TRACK_H_

#include "./app_def.h"
#include "./app_state.h"
#include "./app_state_base.h"
#include <app_core_file.h>
#include <zic_seq_loopMaster.h>

#define APP_TRACK_STATE_SIZE 8

class App_State_Track: public App_State_Base {
protected:
    // Do not change order or remove state keys!!
    enum {
        APP_STATE_PATCH_FILENAME = 0,
        APP_STATE_PRESET,
        APP_STATE_PATTERN,
        APP_STATE_DETUNE,
        APP_STATE_CONDITION
    };

    void setEmptyPatch()
    {
        strcpy(patchFilename, "--");
    }

public:
    char patchFilename[40];
    uint8_t preset = 0;
    Zic_Seq_PatternComponent component;

    App_State_Track()
    {
        setEmptyPatch();
    }

    void setNextPatch(int8_t direction, const char * folder)
    {
        if (isPatchEmpty()) {
            if (direction == 1 && !firstFile(patchFilename, folder, 40)) {
                setEmptyPatch();
                return;
            }
        } else if (!nextFile(patchFilename, folder, patchFilename, direction, 40) && direction == -1) {
            setEmptyPatch();
            return;
        }
    }

    bool isPatchEmpty()
    {
        return patchFilename[0] == '-' && patchFilename[1] == '-';
    }

    void load(char* buffer)
    {
        char key[4];
        memcpy(key, buffer, 3);
        key[3] = '\0';

        switch (atoi(key)) {
        case APP_STATE_PATCH_FILENAME:
            memcpy(patchFilename, buffer + 4, sizeof(patchFilename) - 1);
            removeTrailingSpaces(patchFilename);
            break;

        case APP_STATE_PRESET:
            preset = atoi(buffer + 4);
            break;

        case APP_STATE_PATTERN: {
            if (buffer[4] == '-') {
                component.pattern = NULL;
            } else {
                component.pattern = &App_State::getInstance()->pattern.patterns[atoi(buffer + 4)];
            }
            break;
        }

        case APP_STATE_DETUNE:
            component.detune = atoi(buffer + 4);
            break;

        case APP_STATE_CONDITION:
            component.condition = atoi(buffer + 4);
            break;

        default:
            break;
        }
    }

    void save(Zic_File* file)
    {
        write(file, APP_STRING, APP_STATE_PATCH_FILENAME, patchFilename);
        write(file, APP_NUMBER, APP_STATE_PRESET, &preset);
        if (component.pattern) {
            write(file, APP_NUMBER, APP_STATE_PATTERN, &component.pattern->id);
        } else {
            write(file, APP_NULL, APP_STATE_PATTERN);
        }
        write(file, APP_NUMBER, APP_STATE_DETUNE, &component.detune);
        write(file, APP_NUMBER, APP_STATE_CONDITION, &component.condition);

        write(file, APP_NEXT);
    }
};

#endif
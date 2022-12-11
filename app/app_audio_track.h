#ifndef APP_AUDIO_TRACK_H_
#define APP_AUDIO_TRACK_H_

#include "./app_def.h"
#include "./app_instrument.h"

#include <PdBase.hpp>
#include <PdObject.h>
#include <app_core_file.h>
#include <wavetables/wavetable_Bank.h>
#include <zic_effect_delay.h>
#include <zic_seq_loopMaster.h>

#define APP_TRACK_STATE_SIZE 8
#define APP_STATE_BUFFER 64

#define APP_NEXT 0
#define APP_NULL 1
#define APP_STRING 2
#define APP_NUMBER 3

class App_Audio_TrackState {
protected:
    void setEmptyPatch()
    {
        strcpy(patchFilename, "--");
    }

public:
    char patchFilename[40];
    uint8_t preset = 0;

    App_Audio_TrackState()
    {
        setEmptyPatch();
    }

    void setNextPatch(int8_t direction)
    {
        if (isPatchEmpty()) {
            if (direction == 1 && !firstFile(patchFilename, "instruments", 40)) {
                setEmptyPatch();
                return;
            }
        } else if (!nextFile(patchFilename, "instruments", patchFilename, direction, 40) && direction == -1) {
            setEmptyPatch();
            return;
        }
    }

    bool isPatchEmpty()
    {
        return patchFilename[0] == '-' && patchFilename[1] == '-';
    }
};

class App_Audio_Track {
protected:
    Zic_Seq_Step* stepOff[INSTRUMENT_COUNT];
    const float tickDivider = 1.0f / (256.0f * APP_CHANNELS);
    PdObject pdObject;

    void writeState(Zic_File* file, uint8_t type, uint16_t key = 0, void* ptr = NULL)
    {
        // Fill empty space from buffer with empty space
        // to allow to change variable format without breaking
        char buffer[APP_STATE_BUFFER];
        memset(buffer, ' ', APP_STATE_BUFFER);
        buffer[APP_STATE_BUFFER - 1] = '\n';
        int len = -1;

        if (type != APP_NEXT) {
            if (ptr == NULL || type == APP_NULL) {
                len = sprintf(buffer, "%03d --", key);
            } else if (type == APP_STRING) {
                len = sprintf(buffer, "%03d %s", key, (char*)ptr);
            } else if (type == APP_NUMBER) {
                len = sprintf(buffer, "%03d %d", key, *(uint8_t*)ptr);
            }
            if (len > 0 && len < APP_STATE_BUFFER) {
                buffer[len] = ' ';
            }
        }

        file->write(buffer, APP_STATE_BUFFER);
    }

public:
    uint8_t id = 0;
    char statePath[50];

    Zic_Seq_PatternComponent components[APP_TRACK_STATE_SIZE];
    App_Audio_TrackState state[APP_TRACK_STATE_SIZE];

    Zic_Seq_LoopMaster looper;
    pd::PdBase pd;
    pd::Patch patch;

    App_Audio_Track(uint8_t _id = 0)
        : pdObject(_id)
        , looper(&components[0], APP_TRACK_STATE_SIZE)
    {
        id = _id;
        sprintf(statePath, "projects/current/track_%d.zic", id);
        loadState();
        if (!pd.init(0, APP_CHANNELS, SAMPLE_RATE)) {
            APP_LOG("Could not init pd\n");
        }
        loadPatch();
        pd.setReceiver(&pdObject);
        pd.setMidiReceiver(&pdObject);
    }

    ~App_Audio_Track()
    {
        pd.closePatch(patch);
        pd.clear();
    }

    void next()
    {
        looper.next();

        // TODO need to handle slide
        for (uint8_t i = 0; i < INSTRUMENT_COUNT; i++) {
            if (stepOff[i] && !stepOff[i]->slide) {
                // printf("note off %d\n", stepOff[i]->note);
                pd.sendNoteOn(1, stepOff[i]->note, 0);
                stepOff[i] = NULL;
            }
            if (looper.state.playing && looper.stepOn != 255) {
                Zic_Seq_Step* step = &looper.state.pattern->steps[i][looper.stepOn];
                if (step->note > 0) {
                    // printf("note on %d (%d)\n", step->note, step->velocity);
                    pd.sendNoteOn(1, step->note, step->velocity);
                    stepOff[i] = step;
                }
            }
        }
    }

    void sample(float* buf, int len)
    {
        int ticks = len * tickDivider;
        pd.processFloat(ticks, NULL, buf);
    }

    void cc(uint8_t num, uint8_t val)
    {
        pd.sendControlChange(1, num, val);
    }

    // Do not change order or remove state keys!!
    enum {
        APP_STATE_PATCH_FILENAME = 0,
        APP_STATE_PRESET,
        APP_STATE_PATTERN,
        APP_STATE_DETUNE,
        APP_STATE_CONDITION
    };

    void loadState()
    {
        APP_LOG("Load state %s\n", statePath);

        Zic_File file(statePath, "r");
        if (!file.isOpen()) {
            APP_LOG("Could not open file %s\n", statePath);
            return;
        }

        char buffer[APP_STATE_BUFFER];
        uint8_t idx = 0;
        while (file.read(buffer, APP_STATE_BUFFER) > 0) {
            if (buffer[0] == ' ') {
                idx++;
                continue;
            }
            char key[4];
            memcpy(key, buffer, 3);
            key[3] = '\0';

            switch (atoi(key)) {
            case APP_STATE_PATCH_FILENAME:
                memcpy(state[idx].patchFilename, buffer + 4, sizeof(state[idx].patchFilename) - 1);
                break;

            case APP_STATE_PRESET: 
                state[idx].preset = atoi(buffer + 4);
                break;

            default:
                break;
            }
        }

        file.close();
    }

    void saveState()
    {
        APP_LOG("Save state %s\n", statePath);

        Zic_File file(statePath, "w");
        if (!file.isOpen()) {
            APP_LOG("Could not open file %s\n", statePath);
            return;
        }

        for (uint8_t i = 0; i < APP_TRACK_STATE_SIZE; i++) {
            writeState(&file, APP_STRING, APP_STATE_PATCH_FILENAME, state[i].patchFilename);
            writeState(&file, APP_NUMBER, APP_STATE_PRESET, &state[i].preset);
            if (components[i].pattern) {
                writeState(&file, APP_NUMBER, APP_STATE_PATTERN, &components[i].pattern->id);
            } else {
                writeState(&file, APP_NULL, APP_STATE_PATTERN);
            }
            writeState(&file, APP_NUMBER, APP_STATE_DETUNE, &components[i].detune);
            writeState(&file, APP_NUMBER, APP_STATE_CONDITION, &components[i].condition);

            writeState(&file, APP_NEXT);
        }

        file.close();
    }

    bool isCurrentState(uint8_t pos)
    {
        return looper.getCurrentComponent() == pos;
    }

    void loadPatch()
    {
        // TODO skip if different... valid if last one
        // or if changing of row
        if (patch.isValid()) {
            pd.closePatch(patch);
        }

        uint8_t currentState = looper.getCurrentComponent();
        if (state[currentState].isPatchEmpty()) {
            pd.computeAudio(false);
            return;
        }
        char path[256];
        sprintf(path, "instruments/%s", state[currentState].patchFilename);
        pd.computeAudio(true);
        patch = pd.openPatch("main.pd", path);

        // TODO load preset!!
    }
};

#endif
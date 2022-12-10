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

    void loadState()
    {
        // for (uint8_t i = 0; i < APP_TRACK_STATE_SIZE; i++) {
        //     strcpy(state[i].patchFilename, "--\0");
        // }
    }

    void saveState()
    {
        APP_LOG("save state %s\n", statePath);

        Zic_File file(statePath, "w");
        if (!file.isOpen()) {
            APP_LOG("Could not open file %s\n", statePath);
            return;
        }

        char buffer[256];
        for (uint8_t i = 0; i < APP_TRACK_STATE_SIZE; i++) {
            sprintf(buffer, "%-40s\n", state[i].patchFilename);
            file.write(buffer, 41);
            sprintf(buffer, "%3d\n", state[i].preset);
            file.write(buffer, 4);
            if (components[i].pattern) {
                sprintf(buffer, "%02X\n", components[i].pattern->id);
            } else {
                sprintf(buffer, "--\n");
            }
            file.write(buffer, 3);
            sprintf(buffer, "%2d\n", components[i].detune);
            file.write(buffer, 3);
            sprintf(buffer, "%2d\n", components[i].condition);
            file.write(buffer, 3);

            file.write((void*)"\n", 1);
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
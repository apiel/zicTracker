#ifndef APP_AUDIO_TRACK_H_
#define APP_AUDIO_TRACK_H_

#include "./app_def.h"
#include "./app_instrument.h"

#include "./app_state.h"
#include "./app_state_track.h"
#include <app_core_file.h>
#include <zic_seq_loopMaster.h>

class App_Audio_Track {
protected:
    Zic_Seq_Step* stepOff[VOICE_COUNT];
    char statePath[50];

    Zic_Seq_PatternComponent* components[APP_TRACK_STATE_SIZE] = {
        &state[0].component,
        &state[1].component,
        &state[2].component,
        &state[3].component
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
            state[idx].load(buffer);
        }

        file.close();
    }

public:
    uint8_t id = 0;

    App_State_Track state[APP_TRACK_STATE_SIZE];
    Zic_Seq_LoopMaster looper;

    App_Audio_Track(uint8_t _id = 0)
        : looper(components[0], APP_TRACK_STATE_SIZE)
    {
        id = _id;
        sprintf(statePath, "projects/current/track_%d.zic", id);
        loadState();
    }

    virtual void noteOn(uint8_t note, uint8_t velocity) = 0;
    virtual void noteOff(uint8_t note) = 0;
    virtual void sample(float* buf, int len) = 0;

    virtual void cc(uint8_t num, uint8_t val) { }
    virtual void loadPatch() { }

    virtual bool isOn()
    {
        return false;
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
            state[i].save(&file);
        }

        file.close();
    }

    bool isCurrentState(uint8_t pos)
    {
        return looper.getCurrentComponent() == pos;
    }

    void next()
    {
        looper.next();

        // TODO need to handle tie
        for (uint8_t i = 0; i < VOICE_COUNT; i++) {
            if (stepOff[i] && !stepOff[i]->tie) {
                // printf("note off %d\n", stepOff[i]->note);
                noteOff(stepOff[i]->note);
                stepOff[i] = NULL;
            }
            if (looper.state.playing && looper.stepOn != 255) {
                Zic_Seq_Step* step = &looper.state.pattern->steps[i][looper.stepOn];
                if (step->note > 0) {
                    noteOn(step->note, step->velocity);
                    stepOff[i] = step;
                }
            }
        }
    }
};

#endif
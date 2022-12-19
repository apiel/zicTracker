#ifndef APP_AUDIO_TRACK_H_
#define APP_AUDIO_TRACK_H_

#include "./app_def.h"
#include "./app_instrument.h"

#include "./app_state.h"
#include "./app_state_track.h"
#include "./app_synth.h"
#include <PdBase.hpp>
#include <PdObject.h>
#include <app_core_file.h>
#include <wavetables/wavetable_Bank.h>
#include <zic_effect_delay.h>
#include <zic_seq_loopMaster.h>

// #define APP_TEST_SKIP 4
#define APP_TEST_SKIP 0

class App_Audio_Track {
protected:
    Zic_Seq_Step* stepOff[VOICE_COUNT];
    const float tickDivider = 1.0f / (256.0f * APP_CHANNELS);
    PdObject pdObject;

public:
    uint8_t id = 0;
    char statePath[50];

    App_State_Track state[APP_TRACK_STATE_SIZE];
    Zic_Seq_PatternComponent* components[APP_TRACK_STATE_SIZE] = {
        &state[0].component,
        &state[1].component,
        &state[2].component,
        &state[3].component
    };

    Zic_Seq_LoopMaster looper;
    pd::PdBase pd;
    pd::Patch patch;

    App_Synth synth;

    App_Audio_Track(uint8_t _id = 0)
        : pdObject(_id)
        , looper(components[0], APP_TRACK_STATE_SIZE)
    {
        id = _id;
        sprintf(statePath, "projects/current/track_%d.zic", id);
        loadState();
        if (id > APP_TEST_SKIP) {
            return;
        }
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

        // TODO need to handle tie
        for (uint8_t i = 0; i < VOICE_COUNT; i++) {
            if (stepOff[i] && !stepOff[i]->tie) {
                // printf("note off %d\n", stepOff[i]->note);
                pd.sendNoteOn(1, stepOff[i]->note, 0);
                synth.noteOff(stepOff[i]->note);
                stepOff[i] = NULL;
            }
            if (looper.state.playing && looper.stepOn != 255) {
                Zic_Seq_Step* step = &looper.state.pattern->steps[i][looper.stepOn];
                if (step->note > 0) {
                    // printf("note on %d (%d)\n", step->note, step->velocity);
                    pd.sendNoteOn(1, step->note, step->velocity);
                    synth.noteOn(step->note, step->velocity);
                    stepOff[i] = step;
                }
            }
        }
    }

    void sample(float* buf, int len)
    {
        if (id > APP_TEST_SKIP) {
            int count = len / 4;
            for (int i = 0; i < count; i++) {
                buf[i] = synth.sample();
#if APP_CHANNELS == 2
// TODO
// FIXME
// or not ?? if sample is mono, then it's ok
// but if it's stereo, then it's not :p
                buf[i + 1] = buf[i];
                i++;
#endif
            }

            return;
        }
        int ticks = len * tickDivider;
        pd.processFloat(ticks, NULL, buf);
    }

    void cc(uint8_t num, uint8_t val)
    {
        pd.sendControlChange(1, num, val);
    }

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
        // patch = pd.openPatch("main.pd", "instruments/02_kick");
        // pd.openPatch("main.pd", "instruments/01_synth");

        // FIXME if patch does not exist then crash!!

        // TODO load preset!!
    }
};

#endif
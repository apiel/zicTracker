#ifndef APP_AUDIO_TRACK_H_
#define APP_AUDIO_TRACK_H_

#include "./app_def.h"
#include "./app_instrument.h"

#include <APP_SDL/sdl2.h>
#include <PdBase.hpp>
#include <PdObject.h>
#include <wavetables/wavetable_Bank.h>
#include <zic_effect_delay.h>
#include <zic_seq_loopMaster.h>

#define PATTERN_COMPONENT_COUNT 9

class App_Audio_Track {
protected:
    Zic_Seq_Step* stepOff[INSTRUMENT_COUNT];
    const float tickDivider = 1.0f / (256.0f * APP_CHANNELS);
    PdObject pdObject;

public:
    uint8_t id = 0;

    Zic_Seq_PatternComponent components[PATTERN_COMPONENT_COUNT];

    Zic_Seq_LoopMaster looper;
    pd::PdBase pd;
    pd::Patch patch;

    App_Audio_Track(uint8_t _id = 0)
        : pdObject(_id)
        , looper(&components[0], PATTERN_COMPONENT_COUNT)
    {
        id = _id;
        if (!pd.init(0, APP_CHANNELS, SAMPLE_RATE)) {
            SDL_Log("Could not init pd\n");
        }
        pd.computeAudio(true);
        patch = pd.openPatch("hello.pd", "puredata/synth01");
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

        for (uint8_t i = 0; i < INSTRUMENT_COUNT; i++) {
            if (stepOff[i] && !stepOff[i]->slide) {
                // synths[i]->asr.off();
                // printf("note off %d\n", i);
                pd.sendNoteOn(1, stepOff[i]->note, 0);
                stepOff[i] = NULL;
            }
            if (looper.state.playing && looper.stepOn != 255) {
                Zic_Seq_Step* step = &looper.state.pattern->steps[i][looper.stepOn];
                if (step->note > 0) {
                    pd.sendNoteOn(1, step->note, step->velocity);
                    stepOff[i] = step;
                    printf("note on %d (%d)\n", step->note, step->velocity);
                    // synths[i]->setStep(step);
                    // if (stepOff && stepOff->slide) {
                    //     synths[i]->asr.slide();
                    // } else {
                    //     synths[i]->asr.on();
                    //     // printf("note on %d\n", i);
                    // }
                }
            }
        }
    }

    void sample(float* buf, int len)
    {
        int ticks = len * tickDivider;
        pd.processFloat(ticks, NULL, buf);
    }
};

#endif
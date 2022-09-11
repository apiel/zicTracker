#ifndef APP_AUDIO_TRACK_H_
#define APP_AUDIO_TRACK_H_

#include "./app_instrument.h"
#include "./app_patterns.h"

#include <wavetables/wavetable_Bank.h>
#include <zic_effect_delay.h>
#include <zic_seq_loopMaster.h>

#define PATTERN_COMPONENT_COUNT 9

class App_Audio_Track {
protected:
    Zic_Seq_Step* stepOn[INSTRUMENT_COUNT];

public:
    uint8_t id = 0;

    Zic_Seq_PatternComponent components[PATTERN_COMPONENT_COUNT];

    App_Instrument synth0, synth1, synth2, synth3;
    App_Instrument* synths[INSTRUMENT_COUNT] = { &synth0, &synth1, &synth2, &synth3 };
    App_Instrument* synth = NULL;
    Zic_Seq_LoopMaster looper;
    Zic_Effect_DelayHistory delayHistory;
    Zic_Effect_Delay delay0, delay1, delay2, delay3, delay4;
    Zic_Effect_Delay* delays[DELAY_COUNT] = { &delay0, &delay1, &delay2, &delay3, &delay4 };
    bool delayEnabled = false;

    App_Audio_Track(App_Patterns* patterns, uint8_t _id = 0)
        : synth0(0)
        , synth1(1)
        , synth2(2)
        , synth3(3)
        , looper(&components[0], PATTERN_COMPONENT_COUNT)
        , delay0(&delayHistory)
        , delay1(&delayHistory)
        , delay2(&delayHistory)
        , delay3(&delayHistory)
        , delay4(&delayHistory)
    {
        id = _id;
        // TODO load pattern from last state saved in project status file
        synth1.set("kick.wav", false)->open();
    }

    void next()
    {
        // TODO think? wouldn't it make sense to just pass the step to the synth and out of the step set params
        // then just trigger noteOn and noteOff

        looper.next();
        // FIXME there is no clear note OFF
        // maybe trigger note off earlier???
        // should note OFF run in the same time as the next step but then we need to handle even more polyphony...
        // or should not off trigger before???
        // actually if there is a not a slide, then the note should stop a little bit earlier?!
        // but if there is a slide, it might or not have a note off?
        // on a piano there would be a note off but on a clarinet there would not be

        for (uint8_t i = 0; i < INSTRUMENT_COUNT; i++) {
            Zic_Seq_Step* stepOff = stepOn[i];
            if (stepOff && !stepOff->slide) {
                synths[i]->asr.off();
            }
            if (looper.state.playing && looper.stepOn != 255) {
                stepOn[i] = &looper.state.pattern->steps[i][looper.stepOn];
                if (stepOn[i]->note > 0) {
                    synths[i]->setStep(stepOn[i]);
                    if (stepOff && stepOff->slide) {
                        synths[i]->asr.slide();
                    } else {
                        synths[i]->asr.on();
                    }
                }
            }
        }
    }

    int16_t sample()
    {
        int16_t s = 0;
        for (uint8_t i = 0; i < INSTRUMENT_COUNT; i++) {
            s += synths[i]->next();
        }
        delayHistory.sample(s);
        return s
            + (delayEnabled ? delay0.sample() + delay1.sample() + delay2.sample() + delay3.sample() + delay4.sample() : 0);
    }

    void toggleDelay()
    {
        delayEnabled = !delayEnabled;
    }
};

#endif
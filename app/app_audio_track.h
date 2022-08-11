#ifndef APP_AUDIO_TRACK_H_
#define APP_AUDIO_TRACK_H_

#include "./app_instrument.h"
#include "./app_patterns.h"

#include <wavetables/wavetable_Bank.h>
#include <zic_effect_delay.h>
#include <zic_seq_loopMaster.h>

#define PATTERN_COMPONENT_COUNT 5

class App_Audio_Track {
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

        // delay0.set(0.5, 0.8, 1.0);
        // delay1.set(1.0, 0.4, 1.0);
        // delay2.set(1.5, 0.3, 1.0);
        // delay3.set(2.0, 0.2, 1.0);
        // delay4.set(2.5, 0.1, 1.0);
    }

    void next()
    {
        looper.next();
        Zic_Seq_Step* stepOff = looper.getNoteOff();
        Zic_Seq_Step* stepOn = looper.getNoteOn();
        // FIXME there is no clear note OFF
        // maybe trigger note off earlier???
        if (stepOff) {
            if (synth) {
                synth->asr.off();
                synth = NULL;
            }
        }
        if (stepOn) {
            synth = synths[stepOn->instrument % INSTRUMENT_COUNT];
            // printf("Note %d instrument %d\n", stepOn->note, stepOn->instrument % INSTRUMENT_COUNT);

            synth->wave.reset();
            synth->wave.setNote(stepOn->note);

            // static int count = 0;
            // printf("%d Freq %.3f Time %.3f Amp %d\n",
            //        count++,
            //        synth->wave.getFrequency(),
            //        synth->wave.getTime(),
            //        synth->wave.getAmplitude());

            if (looper.wasSlide()) {
                synth->asr.slide();
            } else {
                synth->asr.on();
            }
        }
    }

    int16_t sample()
    {
        int16_t s = synth ? synth->next() : 0;
        return delayHistory.sample(s)
            + (delayEnabled ? delay0.sample() + delay1.sample() + delay2.sample() + delay3.sample() + delay4.sample() : 0);
    }

    void toggleDelay()
    {
        delayEnabled = !delayEnabled;
    }
};

#endif
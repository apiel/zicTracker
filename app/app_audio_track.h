#ifndef APP_AUDIO_TRACK_H_
#define APP_AUDIO_TRACK_H_

#include "./app_instrument.h"
#include "./app_patterns.h"

#include <wavetables/wavetable_Bank.h>
#include <zic_seq_loop.h>
#include <zic_effect_delay.h>

class App_Audio_Track {
public:
    uint8_t id = 0;

    App_Instrument synth0, synth1, synth2, synth3;
    App_Instrument* synths[INSTRUMENT_COUNT] = { &synth0, &synth1, &synth2, &synth3 };
    App_Instrument* synth = NULL;
    Zic_Seq_Loop looper;
    Zic_Effect_Delay delay;

    App_Audio_Track(App_Patterns* patterns, uint8_t _id = 0)
        : synth0(0)
        , synth1(1)
        , synth2(2)
        , synth3(3)
        , looper(&patterns->patterns[0])
    {
        id = _id;
        // TODO load pattern from last state saved in project status file
        synth1.set("kick.wav", false)->open();
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
        return delay.sample(s);
    }
};

#endif
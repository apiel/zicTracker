#ifndef APP_AUDIO_TRACK_H_
#define APP_AUDIO_TRACK_H_

#include "./app_patterns.h"

#include <wavetables/wavetable_Bank.h>
#include <zic_seq_loop.h>
#include <zic_synth_file.h>

class App_Audio_Track {
public:
    uint8_t id = 0;

    Zic_Synth_File synth0, synth1, synth2, synth3;
    Zic_Synth_File* synths[INSTRUMENT_COUNT] = { &synth0, &synth1, &synth2, &synth3 };
    Zic_Synth_File* synth = NULL;
    Zic_Seq_Loop looper;

    App_Audio_Track(App_Patterns* patterns, uint8_t _id = 0)
        : looper(&patterns->patterns[0])
    {
        id = _id;
        // TODO load pattern from last state saved in project status file
        synth1.wave.open("samples/kick.wav");
    }

    void next()
    {
        looper.next();
        Zic_Seq_Step* stepOff = looper.getNoteOff();
        Zic_Seq_Step* stepOn = looper.getNoteOn();
        if (stepOff) {
            if (synth) {
                synth->asr.off();
                synth = NULL;
            }
        }
        if (stepOn) {
            synth = synths[stepOn->instrument % INSTRUMENT_COUNT];
            printf("Note %d instrument %d\n", stepOn->note, stepOn->instrument % INSTRUMENT_COUNT);
            synth->wave.restart();
            synth->wave.setNote(stepOn->note);
            synth->asr.on();
        }
    }
};

#endif
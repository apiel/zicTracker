#ifndef APP_AUDIO_TRACK_H_
#define APP_AUDIO_TRACK_H_

#include "./app_instrument.h"
#include "./app_patterns.h"

#include <wavetables/wavetable_Bank.h>
#include <zic_seq_loop.h>

// On teensy we might reduce this significantly
// Or find another way to store it
#ifndef APP_AUDIO_TRACK_HISTORY_SEC
#define APP_AUDIO_TRACK_HISTORY_SEC 5
// #define APP_AUDIO_TRACK_HISTORY_SEC 1
#endif

#define APP_AUDIO_TRACK_HISTORY_SAMPLES (APP_AUDIO_TRACK_HISTORY_SEC * SAMPLE_RATE)

class App_Audio_Track {
public:
    uint8_t id = 0;

    App_Instrument synth0, synth1, synth2, synth3;
    App_Instrument* synths[INSTRUMENT_COUNT] = { &synth0, &synth1, &synth2, &synth3 };
    App_Instrument* synth = NULL;
    Zic_Seq_Loop looper;

    int16_t history[APP_AUDIO_TRACK_HISTORY_SAMPLES] = { 0 };
    uint32_t historyIndex = 0;

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
        int16_t s = synth->next();
        historyIndex = (historyIndex + 1) % APP_AUDIO_TRACK_HISTORY_SAMPLES;
        history[historyIndex] = s;
        // return s;
        return s
            + (history[(historyIndex + APP_AUDIO_TRACK_HISTORY_SAMPLES - (uint32_t)(APP_AUDIO_TRACK_HISTORY_SAMPLES * 0.1)) % APP_AUDIO_TRACK_HISTORY_SAMPLES] * 0.40)
            + (history[(historyIndex + APP_AUDIO_TRACK_HISTORY_SAMPLES - (uint32_t)(APP_AUDIO_TRACK_HISTORY_SAMPLES * 0.2)) % APP_AUDIO_TRACK_HISTORY_SAMPLES] * 0.35)
            + (history[(historyIndex + APP_AUDIO_TRACK_HISTORY_SAMPLES - (uint32_t)(APP_AUDIO_TRACK_HISTORY_SAMPLES * 0.3)) % APP_AUDIO_TRACK_HISTORY_SAMPLES] * 0.30)
            + (history[(historyIndex + APP_AUDIO_TRACK_HISTORY_SAMPLES - (uint32_t)(APP_AUDIO_TRACK_HISTORY_SAMPLES * 0.4)) % APP_AUDIO_TRACK_HISTORY_SAMPLES] * 0.25)
            + (history[(historyIndex + APP_AUDIO_TRACK_HISTORY_SAMPLES - (uint32_t)(APP_AUDIO_TRACK_HISTORY_SAMPLES * 0.5)) % APP_AUDIO_TRACK_HISTORY_SAMPLES] * 0.20)
            + (history[(historyIndex + APP_AUDIO_TRACK_HISTORY_SAMPLES - (uint32_t)(APP_AUDIO_TRACK_HISTORY_SAMPLES * 0.6)) % APP_AUDIO_TRACK_HISTORY_SAMPLES] * 0.15);

            // + (history[(historyIndex + (uint32_t)(APP_AUDIO_TRACK_HISTORY_SAMPLES * 0.9)) % APP_AUDIO_TRACK_HISTORY_SAMPLES] * 0.40)
            // + (history[(historyIndex + (uint32_t)(APP_AUDIO_TRACK_HISTORY_SAMPLES * 0.8)) % APP_AUDIO_TRACK_HISTORY_SAMPLES] * 0.35)
            // + (history[(historyIndex + (uint32_t)(APP_AUDIO_TRACK_HISTORY_SAMPLES * 0.7)) % APP_AUDIO_TRACK_HISTORY_SAMPLES] * 0.30)
            // + (history[(historyIndex + (uint32_t)(APP_AUDIO_TRACK_HISTORY_SAMPLES * 0.6)) % APP_AUDIO_TRACK_HISTORY_SAMPLES] * 0.25)
            // + (history[(historyIndex + (uint32_t)(APP_AUDIO_TRACK_HISTORY_SAMPLES * 0.5)) % APP_AUDIO_TRACK_HISTORY_SAMPLES] * 0.20)
            // + (history[(historyIndex + (uint32_t)(APP_AUDIO_TRACK_HISTORY_SAMPLES * 0.4)) % APP_AUDIO_TRACK_HISTORY_SAMPLES] * 0.15);
    }
};

#endif
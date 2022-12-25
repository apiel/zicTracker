#ifndef APP_AUDIO_TRACK_SYNTH_H_
#define APP_AUDIO_TRACK_SYNTH_H_

#include "./app_audio_track.h"
#include "./app_synth.h"

class App_Audio_TrackSynth : public App_Audio_Track {
public:
    App_Synth synth;

    App_Audio_TrackSynth(uint8_t _id = 0)
        : App_Audio_Track(_id)
    {
    }

    void noteOn(uint8_t note, uint8_t velocity, uint8_t voice)
    {
        synth.noteOn(note, velocity);
    }

    void noteOff(uint8_t note, uint8_t voice)
    {
        synth.noteOff(note);
    }

    void sample(float* buf, int len)
    {
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
    }

    void cc(uint8_t num, uint8_t val, uint8_t voice) override
    {
        if (num == 1) {
            // synth set wavetable
        } else if (num == 2) {
            // synth.osc.morph
        } else if (num == 3) {
            // synth.osc.setFrequency();
        } else if (num == 4) {
            // synth.osc.setAmplitude(val / 127.0f);
        } else if (num == 5) {
            synth.adsr.setAttack(val / 127.0f);
        } else if (num == 6) {
            synth.adsr.setDecay(val / 127.0f);
        } else if (num == 7) {
            synth.adsr.setSustain(val / 127.0f);
        } else if (num == 8) {
            synth.adsr.setRelease(val / 127.0f);
        } else if (num == 9) {
        } else if (num == 10) {
        } else if (num == 11) {
        } else if (num == 12) {
        } else if (num == 13) {
        } else if (num == 14) {
        } else if (num == 15) {
        } else if (num == 16) {
        }
    }
};

#endif
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
};

#endif
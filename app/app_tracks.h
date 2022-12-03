#ifndef APP_TRACKS_H_
#define APP_TRACKS_H_

#include "./app_audio_track.h"
#include "./app_def.h"

#include <stdlib.h>

#include <wavetables/wavetable_Bank.h>
#include <zic_seq_loop.h>
#include <zic_synth_file.h>

class App_Tracks {
protected:
    const float mixerDivider = 1.0f / TRACK_COUNT;

public:
    uint8_t trackId = TRACK_1;
    App_Audio_Track track0, track1, track2, track3;
    App_Audio_Track* tracks[TRACK_COUNT] = { &track0, &track1, &track2, &track3 };
    App_Audio_Track* track = NULL;

    Zic_Seq_Loop* looper = NULL;

    App_Tracks()
        : track0(TRACK_1)
        , track1(TRACK_2)
        , track2(TRACK_3)
        , track3(TRACK_4)
    {
        select(trackId);

        // just for testing
        // track0.looper.nextState.play();
    }

    void select(uint8_t id)
    {
        trackId = id % TRACK_COUNT;
        track = tracks[trackId];
        looper = &track->looper;
    }

    void next()
    {
        for (uint8_t t = 0; t < TRACK_COUNT; t++) {
            tracks[t]->next();
        }
    }

    void sample(float* buf, int len)
    {
        // tracks[0]->sample(buf, len);

        for (int j = 0; j < len; j++) {
            buf[j] = 0.0f;
        }

        // NOTE should dynamic data allocation?
        // float buffer[APP_AUDIO_CHUNK];
        float* buffer = new float[len];
        for (uint8_t i = 0; i < TRACK_COUNT; i++) {
            tracks[i]->sample(buffer, len);
            for (int j = 0; j < len; j++) {
                buf[j] += buffer[j] * mixerDivider;
            }
        }
        delete[] buffer;
    }

    void togglePlay()
    {
        for (uint8_t i = 0; i < TRACK_COUNT; i++) {
            tracks[i]->looper.togglePlay();
        }
    }

    void reset()
    {
        for (uint8_t i = 0; i < TRACK_COUNT; i++) {
            tracks[i]->looper.reset();
        }
    }

    bool isPlaying()
    {
        return tracks[0]->looper.isPlaying();
    }

    void cc(uint8_t num, uint8_t val)
    {
        // TODO should we select the right track?
        for (uint8_t i = 0; i < TRACK_COUNT; i++) {
            tracks[i]->cc(num, val);
        }
    }
};

#endif
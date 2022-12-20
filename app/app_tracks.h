#ifndef APP_TRACKS_H_
#define APP_TRACKS_H_

#include "./app_audio_track.h"
#include "./app_audio_trackPd.h"
#include "./app_audio_trackSynth.h"
#include "./app_def.h"

#include <stdlib.h>

#include <wavetables/wavetable_Bank.h>
#include <zic_seq_loop.h>
#include <zic_synth_file.h>

#define APP_PD_TRACKS 1

class App_Tracks {
protected:
    const float mixerDivider = 1.0f / TRACK_COUNT;

    App_Tracks()
        : track0(TRACK_1)
        , track1(TRACK_2)
        , track2(TRACK_3)
        , track3(TRACK_4)
    {
    }

public:
    uint8_t trackId = TRACK_1;

#if APP_PD_TRACKS == 1
    App_Audio_TrackPd track0;
    App_Audio_TrackSynth track1, track2, track3;
#else
    App_Audio_TrackPd track0, track1;
    App_Audio_TrackSynth track2, track3;
#endif

    App_Audio_Track* tracks[TRACK_COUNT] = { &track0, &track1, &track2, &track3 };

    static App_Tracks* instance;

    static App_Tracks* getInstance()
    {
        if (!instance) {
            instance = new App_Tracks();
        }
        return instance;
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

    void quit()
    {
        for (uint8_t i = 0; i < TRACK_COUNT; i++) {
            tracks[i]->saveState();
        }
    }
};

App_Tracks* App_Tracks::instance = NULL;

#endif
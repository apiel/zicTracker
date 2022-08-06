#ifndef APP_TRACKS_H_
#define APP_TRACKS_H_

#include "./app_audio_track.h"
#include "./app_def.h"
#include "./app_patterns.h"

#include <stdlib.h>

#include <wavetables/wavetable_Bank.h>
#include <zic_seq_loop.h>
#include <zic_synth_file.h>

class App_Tracks {
public:
    App_Patterns* patterns;
    uint8_t trackId = TRACK_1;
    App_Audio_Track track0, track1, track2, track3;
    App_Audio_Track* tracks[TRACK_COUNT] = { &track0, &track1, &track2, &track3 };
    App_Audio_Track* track = NULL;

    Zic_Seq_Loop* looper = NULL;

    App_Tracks(App_Patterns* _patterns)
        : patterns(_patterns)
        , track0(_patterns, TRACK_1)
        , track1(_patterns, TRACK_2)
        , track2(_patterns, TRACK_3)
        , track3(_patterns, TRACK_4)
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

    int16_t sample()
    {
        int16_t sample = 0;
        for (uint8_t t = 0; t < TRACK_COUNT; t++) {
            if (tracks[t]->synth) {
                sample += tracks[t]->synth->next();
            }
        }
        return sample;
    }
};

#endif
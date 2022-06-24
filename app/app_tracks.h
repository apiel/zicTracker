#ifndef APP_TRACKS_H_
#define APP_TRACKS_H_

#include "./app_def.h"
#include "./app_patterns.h"
#include "./app_audio_track.h"

#include <stdlib.h>

#include <zic_wavetable_synth.h>
#include <zic_seq_loop.h>
#include <wavetables/wavetable_Bank.h>

class App_Tracks
{
public:
    uint8_t trackId = TRACK_1;
    App_Audio_Track track0, track1, track2, track3;
    App_Audio_Track *tracks[TRACK_COUNT] = {&track0, &track1, &track2, &track3};
    App_Audio_Track *track = NULL;
    Zic_Wavetable_Synth *synth = NULL;
    Zic_Seq_Loop *looper = NULL;

    App_Tracks(App_Patterns* patterns) : track0(TRACK_1), track1(TRACK_2), track2(TRACK_3), track3(TRACK_4)
    {
        select(trackId);
    }

    void select(uint8_t id)
    {
        trackId = id % TRACK_COUNT;
        track = tracks[trackId];
        synth = &track->synth;
        looper = &track->looper;
    }

    void next()
    {
        for (uint8_t t = 0; t < TRACK_COUNT; t++)
        {
            tracks[t]->next();
        }
    }

    int16_t sample()
    {
        // return track0.synth.next();
        // return track0.synth.next() + track1.synth.next(); // + track2.synth.next() + track3.synth.next();
        return track0.synth.next() + track1.synth.next() + track2.synth.next() + track3.synth.next();
    }
};

#endif
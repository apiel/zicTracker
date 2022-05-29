#ifndef APP_H_
#define APP_H_

#include "./app_tracks.h"
#include <zic_seq_tempo.h>

// #define TEMPO_CUSTOM_TIME 1


class App {
public:
    Zic_Seq_Tempo<> tempo;
    App_Tracks tracks;

    void start()
    {
        tracks.looper->setLoopMode(true);
        tracks.looper->on(60);
    }

    int16_t sample()
    {
#ifndef TEMPO_CUSTOM_TIME
        if (tempo.next()) {
            tracks.next();
        }
#endif
        return tracks.sample();
    }

    void next(unsigned long now)
    {
#ifdef TEMPO_CUSTOM_TIME
        if (tempo.next(now)) {
            tracks.next();
        }
#endif
    }
};

#endif
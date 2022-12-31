#ifndef APP_MIDI_H_
#define APP_MIDI_H_

#include <RtMidi.h>
#include <stdint.h>

#include "./app_tracks.h"

class App_Midi {
protected:
    App_Midi()
    {
        RtMidi::getCompiledApi(apis);
        setNextApi(0);
    }

public:
    std::vector<RtMidi::Api> apis;
    uint8_t currentApi = 0;

    static App_Midi* instance;

    static App_Midi* getInstance()
    {
        if (!instance) {
            instance = new App_Midi();
        }
        return instance;
    }

    const char* getApiName()
    {
        return RtMidi::getApiDisplayName(apis[currentApi]).c_str();
    }

    void setNextApi(int8_t direction)
    {
        currentApi = (currentApi + direction + apis.size()) % apis.size();

        App_Tracks* tracks = App_Tracks::getInstance();
        // FIXME use loop to set midiout for all midi tracks
        // for (uint8_t i = tracks->TRACK_AUDIO_COUNT; i < tracks->TRACK_MIDI_COUNT; i++)
        // {
        //     /* code */
        // }
        tracks->track4.setMidiOut(new RtMidiOut(apis[currentApi]));
        tracks->track5.setMidiOut(new RtMidiOut(apis[currentApi]));
        tracks->track6.setMidiOut(new RtMidiOut(apis[currentApi]));
        tracks->track7.setMidiOut(new RtMidiOut(apis[currentApi]));
    }
};

App_Midi* App_Midi::instance = NULL;

#endif

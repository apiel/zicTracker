#ifndef APP_MIDI_H_
#define APP_MIDI_H_

#include <RtMidi.h>
#include <stdint.h>

#include "./app_tracks.h"

// we should set midi output from project settings

class App_Midi {
protected:
    App_Midi()
    {
        RtMidi::getCompiledApi(apis);
        setNextApi(0);

        // for (unsigned int i = 0; i < apis.size(); i++) {
        //     RtMidiIn* midiin = 0;
        //     RtMidiOut* midiout = 0;
        //     try {
        //         printf("Current API: %s\n", RtMidi::getApiDisplayName(apis[i]).c_str());

        //         midiin = new RtMidiIn(apis[i]);
        //         unsigned int nPorts = midiin->getPortCount();
        //         if (nPorts) {
        //             for (unsigned i = 0; i < nPorts; i++) {
        //                 std::cout << "   Input Port #" << i << ": " << midiin->getPortName(i) << '\n';
        //             }
        //         }

        //         midiout = new RtMidiOut(apis[i]);
        //         nPorts = midiout->getPortCount();
        //         if (nPorts) {
        //             for (unsigned i = 0; i < nPorts; i++) {
        //                 std::cout << "   Output Port #" << i << ": " << midiout->getPortName(i) << std::endl;
        //             }
        //         }
        //         std::cout << std::endl;
        //     } catch (RtMidiError& error) {
        //         // error.printMessage();
        //     }

        //     delete midiin;
        //     delete midiout;
        // }
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

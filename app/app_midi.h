#ifndef APP_MIDI_H_
#define APP_MIDI_H_

#include <RtMidi.h>
#include <stdint.h>

// we should set midi output from project settings

class App_Midi {
protected:
    App_Midi()
    {
        std::vector<RtMidi::Api> apis;
        RtMidi::getCompiledApi(apis);

        for (unsigned int i = 0; i < apis.size(); i++) {
            RtMidiIn* midiin = 0;
            RtMidiOut* midiout = 0;
            try {
                printf("Current API: %s\n", RtMidi::getApiDisplayName(apis[i]).c_str());

                midiin = new RtMidiIn(apis[i]);
                unsigned int nPorts = midiin->getPortCount();
                if (nPorts) {
                    for (unsigned i = 0; i < nPorts; i++) {
                        std::cout << "   Input Port #" << i << ": " << midiin->getPortName(i) << '\n';
                    }
                }

                midiout = new RtMidiOut(apis[i]);
                nPorts = midiout->getPortCount();
                if (nPorts) {
                    for (unsigned i = 0; i < nPorts; i++) {
                        std::cout << "   Output Port #" << i << ": " << midiout->getPortName(i) << std::endl;
                    }
                }
                std::cout << std::endl;
            } catch (RtMidiError& error) {
                // error.printMessage();
            }

            delete midiin;
            delete midiout;
        }
    }

public:
    static App_Midi* instance;

    static App_Midi* getInstance()
    {
        if (!instance) {
            instance = new App_Midi();
        }
        return instance;
    }
};

App_Midi* App_Midi::instance = NULL;

#endif

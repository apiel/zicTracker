#ifndef APP_INSTRUMENT_H_
#define APP_INSTRUMENT_H_

#include <zic_synth_file.h>

class App_Instrument : public Zic_Synth_File {
public:
    // App_Instrument()
    // {
    //     // open("samples/kick.wav");
    //     open("wavetables/59.wav", true);
    // }
    
    const char* getFolder()
    {
        if (wave.isWavetable) {
            return "wavetables";
        }
        return "samples";
    }
};

#endif
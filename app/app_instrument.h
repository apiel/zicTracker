#ifndef APP_INSTRUMENT_H_
#define APP_INSTRUMENT_H_

#include <zic_synth_file.h>

class App_Instrument : public Zic_Synth_File {
public:
    char filename[256] = "59.wav";
    bool isWavetable = true;

    App_Instrument()
    {
        // TODO load filename from last state
        open();
    }

    App_Instrument* set(const char* name, bool _isWavetable)
    {
        strcpy(filename, name);
        isWavetable = _isWavetable;
        return this;
    }

    App_Instrument* open()
    {
        char filepath[270];
        snprintf(filepath, 270, "%s/%s", getFolder(), filename);
        wave.open(filepath, isWavetable);

        return this;
    }

    const char* getFolder()
    {
        if (isWavetable) {
            return "wavetables";
        }
        return "samples";
    }
};

#endif
#ifndef APP_INSTRUMENT_H_
#define APP_INSTRUMENT_H_

#include <zic_synth_file.h>

#include "./app_file.h"

class App_Instrument : public Zic_Synth_File {
public:
    uint8_t id = 0;
    char filename[256] = "59.wav";
    bool isWavetable = true;

    App_Instrument(uint8_t _id = 0)
        : id(_id)
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

    App_Instrument * setNext(int8_t direction = 1)
    {
        nextFile(filename, getFolder(), filename, direction);
        // open();
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
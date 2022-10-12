#ifndef APP_INSTRUMENT_H_
#define APP_INSTRUMENT_H_

#include <zic_seq_pattern.h>
#include <zic_synth_file.h>

#include <app_core_file.h>

class App_Instrument : public Zic_Synth_File {
protected:
    int8_t mutedAmplitude = 0;

public:
    uint8_t id = 0;
    // char filename[256] = "1.wav";
    // char filename[256] = "59.wav";
    // char filename[256] = {'5','9','.','w', 'a', 'v','\0'};
    // char filename[256] = "000testyo.wav";
    // char filename[256] = "000GRAV-A1.WAV";
    // char filename[256] = "303.WAV";
    // char filename[256] = "111.WAV";
    char filename[256] = "BANK_A.WAV";
    // char filename[256] = "_additive.wav";
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

    App_Instrument* setNext(int8_t direction = 0)
    {
        nextFile(filename, getFolder(), filename, direction);
        open();
        return this;
    }

    App_Instrument* setFirst()
    {
        firstFile(filename, getFolder());
        open();
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

    void setStep(Zic_Seq_Step* _step, int8_t detune = 0)
    {
        wave.reset();
        wave.setNote(_step->note + detune);
        wave.setVelocity(_step->velocity);
    }
};

#endif
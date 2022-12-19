#ifndef APP_SYNTH_H_
#define APP_SYNTH_H_

#include <zic_effect_filter.h>
#include <zic_mod_adsr.h>

#include "./app_wavetable.h"

#define APP_SYNTH_WAVE_COUNT 4
#define APP_SYNTH_ENV_COUNT 2
#define APP_SYNTH_PATCHE_COUNT 4

class App_Synth {
public:
    App_Wavetable wavetable[APP_SYNTH_WAVE_COUNT];
    Zic_Effect_Filter filter;
    Zic_Mod_Adsr adsr[APP_SYNTH_ENV_COUNT];

    float sample()
    {
        float oscOut = wavetable[0].next();
        float envOut = adsr[0].next();

        // might need to convert to float!!!!
        // return filter.next(oscOut * envOut);

        return oscOut * envOut;
    }

    void noteOn(uint8_t note, uint8_t velocity)
    {
        wavetable[0].setFrequency(Zic::NOTE_FREQ[note]);
        // printf("App_Synth::noteOn: %f\n", Zic::NOTE_FREQ[note]);
        adsr[0].on();
    }

    void noteOff(uint8_t note)
    {
        adsr[0].off();
    }
};

// the mixer should be done in the part while assigning patches
//
// osc in lfo mode could be run in one shot, for example for kick design
// therefor would need a way to design custom wavetable :p
// maybe we should think to use linear interpolation for the wavetable to have smoother transitions

// Use Y button to trigger noteOn noteOff
// L1/R1 could be to switch the note to play?? or not :p

// would be great that at the end of a phase of the OSC to trigger a something
// e.g. trigger a random pitch change
// or trigger pitch inc?

#endif
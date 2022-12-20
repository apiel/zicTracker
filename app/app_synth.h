#ifndef APP_SYNTH_H_
#define APP_SYNTH_H_

#include <zic_effect_filter.h>
#include <zic_mod_adsr.h>

#include "./app_wavetable.h"

#define APP_SYNTH_OSC_COUNT 2
#define APP_SYNTH_ENV_COUNT 2
#define APP_SYNTH_PATCHE_COUNT 4

class App_Synth {
public:
    App_Wavetable osc[APP_SYNTH_OSC_COUNT];
    Zic_Effect_Filter filter;
    Zic_Mod_Adsr adsr[APP_SYNTH_ENV_COUNT];

    float sample()
    {
        float envOut = adsr[0].next();

        // might need to convert to float!!!!
        // return filter.next(oscOut * envOut);

        // return (osc[0].next() * 0.5 + osc[1].next() * 0.5) * envOut;
        return osc[0].next() * envOut;
    }

    void noteOn(uint8_t note, uint8_t velocity)
    {
        osc[0].setFrequency(Zic::NOTE_FREQ[note]);
        osc[1].setFrequency(Zic::NOTE_FREQ[note]);
        // printf("App_Synth::noteOn: %f\n", Zic::NOTE_FREQ[note]);
        adsr[0].on();
    }

    void noteOff(uint8_t note)
    {
        adsr[0].off();
    }
};

#endif
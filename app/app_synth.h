#ifndef APP_SYNTH_H_
#define APP_SYNTH_H_

#include <zic_effect_filter.h>
#include <zic_mod_adsr.h>

#include "./app_wavetable.h"

#define APP_SYNTH_OSC_COUNT 2
#define APP_SYNTH_LFO_COUNT 2
#define APP_SYNTH_ENV_COUNT 2
#define APP_SYNTH_PATCHE_COUNT 4

class App_Synth {
protected:
    enum {
        MOD_TARGET_CUTOFF = 0,
        MOD_TARGET_RES,
        MOD_TARGET_PITCH_1,
        MOD_TARGET_PITCH_2,
        MOD_TARGET_AMP_1,
        MOD_TARGET_AMP_2,
        MOD_TARGET_MORPH_1,
        MOD_TARGET_MORPH_2,
        MOD_TARGET_COUNT
    };

    enum {
        MOD_SRC_ENV_1 = 0,
        MOD_SRC_ENV_2,
        MOD_SRC_LFO_1,
        MOD_SRC_LFO_2,
        MOD_SRC_COUNT
    };

    float modValue[MOD_TARGET_COUNT];
    float modIntensity[MOD_SRC_COUNT][MOD_TARGET_COUNT];
    float modSumIntensity[MOD_SRC_COUNT];

    void updateModIntensity()
    {
        for (int i = 0; i < MOD_SRC_COUNT; i++) {
            modSumIntensity[i] = 0.0;
            for (int j = 0; j < MOD_TARGET_COUNT; j++) {
                modSumIntensity[i] += modIntensity[i][j];
                printf("modIntensity[%d][%d] is %f\n", i, j, modIntensity[i][j]);
            }
            if (modSumIntensity[i] > 0.0) {
                // printf("modSumIntensity[%d] is %f become %f\n", i, modSumIntensity[i], 1 / modSumIntensity[i]);
                modSumIntensity[i] = 1 / modSumIntensity[i];
            }
            // printf("modSumIntensity[%d] is %f\n", i, modSumIntensity[i]);
        }
    }

    void updateModValue(uint8_t src, float value)
    {
        for (uint8_t i = 0; i < MOD_TARGET_COUNT; i++) {
            modValue[i] += value * modIntensity[src][i];
        }
    }

    void resetModValue()
    {
        for (uint8_t i = 0; i < MOD_TARGET_COUNT; i++) {
            modValue[i] = 0.0;
        }
    }

public:
    App_Wavetable osc[APP_SYNTH_OSC_COUNT];
    App_Wavetable lfo[APP_SYNTH_LFO_COUNT];
    Zic_Effect_Filter filter;
    Zic_Mod_Adsr adsr[APP_SYNTH_ENV_COUNT];

    App_Synth()
    {
        // filter.setFrequency(8000);
        // filter.setResonance(0.99);

        updateModIntensity();
        setModIntensity(MOD_SRC_ENV_1, MOD_TARGET_AMP_1, 1.0);
    }

    void setModIntensity(int src, int target, float intensity)
    {
        modIntensity[src][target] = intensity;
        updateModIntensity();
    }

    float sample()
    {
        resetModValue();
        updateModValue(MOD_SRC_ENV_1, adsr[0].next());
        updateModValue(MOD_SRC_ENV_2, adsr[1].next());
        updateModValue(MOD_SRC_LFO_1, lfo[0].next());
        updateModValue(MOD_SRC_LFO_2, lfo[1].next());

        // might need to convert to float!!!!
        // return filter.next(oscOut * envOut);

        // return (osc[0].next() * 0.5 + osc[1].next() * 0.5) * envOut;
        // return osc[0].next() * envOut;

        // if (modValue[MOD_TARGET_AMP_1] > 1.0) {
        //     printf("App_Synth::sample: %f\n", modValue[MOD_TARGET_AMP_1]);
        // }
        return filter.next(osc[0].next() * modValue[MOD_TARGET_AMP_1] * modSumIntensity[MOD_SRC_ENV_1]);
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
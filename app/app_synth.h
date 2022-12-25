#ifndef APP_SYNTH_H_
#define APP_SYNTH_H_

#include <zic_effect_filter.h>
#include <zic_mod_adsr.h>

#include "./app_wavetable.h"

#define APP_SYNTH_LFO_COUNT 2

class App_Synth {
protected:
    enum {
        MOD_TARGET_CUTOFF = 0,
        MOD_TARGET_RES,
        MOD_TARGET_PITCH,
        MOD_TARGET_AMP,
        MOD_TARGET_MORPH,
        MOD_TARGET_COUNT
    };

    const char* modTargetName[MOD_TARGET_COUNT] = {
        "Cutoff",
        "Resonance",
        "Pitch",
        "Amp",
        "Morph",
    };

    enum {
        MOD_SRC_ENV = 0,
        MOD_SRC_LFO_1,
        MOD_SRC_LFO_2,
        MOD_SRC_COUNT
    };

    float modValue[MOD_TARGET_COUNT];
    float modIntensity[MOD_TARGET_COUNT][MOD_SRC_COUNT];
    float modSumIntensity[MOD_TARGET_COUNT] = { 0.0, 0.0, 0.0, 0.0, 0.0 };

    void updateModIntensity()
    {
        for (uint8_t i = 0; i < MOD_TARGET_COUNT; i++) {
            modSumIntensity[i] = 0.0;
            for (uint8_t j = 0; j < MOD_SRC_COUNT; j++) {
                modSumIntensity[i] += modIntensity[i][j];
            }
            if (modSumIntensity[i] > 0.0) {
                modSumIntensity[i] = 1.0 / modSumIntensity[i];
            }
            // printf("modSum %s is %f\n", modTargetName[i], modSumIntensity[i]);
        }
    }

    void updateModValue(uint8_t src, float value)
    {
        for (uint8_t i = 0; i < MOD_TARGET_COUNT; i++) {
            modValue[i] += value * modIntensity[i][src];
        }
    }

    void resetModValue()
    {
        for (uint8_t i = 0; i < MOD_TARGET_COUNT; i++) {
            modValue[i] = 0.0;
        }
    }

public:
    App_Wavetable osc;
    App_Wavetable lfo[APP_SYNTH_LFO_COUNT];
    Zic_Effect_Filter filter;
    Zic_Mod_Adsr adsr;

    App_Synth()
    {
        // filter.setFrequency(8000);
        // filter.setResonance(0.99);

        setModIntensity(MOD_SRC_LFO_1, MOD_TARGET_AMP, 1.0);
        lfo[0].setFrequency(0.5);
    }

    void setModIntensity(uint8_t src, uint8_t target, float intensity)
    {
        modIntensity[target][src] = intensity;
        updateModIntensity();
    }

    float sample()
    {
        resetModValue();

        // Use the first envelope to modulate the signal output
        // and this cannot be changed, however, modulation can
        // still be use with other targets
        float envOut = adsr.next();

        updateModValue(MOD_SRC_ENV, envOut);
        updateModValue(MOD_SRC_LFO_1, lfo[0].next());
        updateModValue(MOD_SRC_LFO_2, lfo[1].next());

        // printf("modValue[MOD_TARGET_AMP_1] is %f intensity %f\n", modValue[MOD_TARGET_AMP_1], modSumIntensity[MOD_TARGET_AMP_1]);

        return filter.next(
                   osc.next(
                       modValue[MOD_TARGET_AMP] * modSumIntensity[MOD_TARGET_AMP],
                       modValue[MOD_TARGET_PITCH] * modSumIntensity[MOD_TARGET_PITCH],
                       modValue[MOD_TARGET_MORPH] * modSumIntensity[MOD_TARGET_MORPH]))
            * envOut;
    }

    void noteOn(uint8_t note, uint8_t velocity)
    {
        osc.setFrequency(Zic::NOTE_FREQ[note]);
        // printf("App_Synth::noteOn: %f\n", Zic::NOTE_FREQ[note]);
        adsr.on();
    }

    void noteOff(uint8_t note)
    {
        adsr.off();
    }
};

#endif
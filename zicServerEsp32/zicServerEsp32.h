#ifndef ZIC_SERVER_ESP32_H_
#define ZIC_SERVER_ESP32_H_

// ESP32 dev module -> audio kit

#include <AudioKitHAL.h>

#include <wavetables/wavetable_Bank.h>
#include <zic_wave_wavetable.h>

AudioKit kit;
const int BUFFER_SIZE = 512;
int16_t buffer[BUFFER_SIZE];

Zic_Wave_Wavetable wave(&wavetable_Bank);

void zicServerEsp32Init()
{
    LOGLEVEL_AUDIOKIT = AudioKitInfo;
    Serial.begin(115200);
    // open in write mode
    AudioKitConfig cfg = kit.defaultConfig(AudioOutput);
    kit.begin(cfg);
}

void zicServerEsp32Loop()
{
    // static union sampleTUNT {
    //     uint32_t sample;
    //     int16_t ch[2];
    // } sampleDataU;

    for (int i = 0; i < BUFFER_SIZE; ++i) {
        // sampleDataU.ch[0] = int16_t(*fr_sample * 16383.0f); /* some bits missing here */
        // sampleDataU.ch[1] = int16_t(*fl_sample * 16383.0f);
        
        buffer[i] = wave.next() * 100;
    }
    kit.write(buffer, BUFFER_SIZE);
}

#endif
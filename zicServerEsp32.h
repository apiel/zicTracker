#ifndef ZIC_SERVER_ESP32_H_
#define ZIC_SERVER_ESP32_H_

// ESP32 dev module -> audio kit

#include <AudioKitHAL.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

// #include <zic_wave_wavetable.h>
// #include <wavetables/wavetable_Bank.h>
#include "app/app.h"

#define SCREEN_W 128 // OLED display width, in pixels
#define SCREEN_H 64 // OLED display height, in pixels

#define PIN_KEYS_COUNT 6

uint8_t pinKeys[PIN_KEYS_COUNT] = { 36, 13, 19, 23, 18, 5 };

AudioKit kit;
App app;

Zic_Wave_Wavetable wave(&wavetable_Bank);

Adafruit_SSD1306 d(SCREEN_W, SCREEN_H, &Wire, -1);

void initDisplay()
{
    // should here just have available ...??
    if (!d.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ; // Don't proceed, loop forever
    }

    d.clearDisplay();
    d.setTextColor(WHITE);
    d.setTextSize(1);
    d.setCursor(0, 0);

    d.println("abcdefghijklmntpqrstu");
    d.display();
}

void buttonLoop()
{
    for (uint8_t p = 0; p < PIN_KEYS_COUNT; p++) {
        if (digitalRead(pinKeys[p]) == LOW) {
            Serial.printf("PIN_KEY %d pressed\n", p);
        }
    }
}

void zicServerEsp32Init()
{
    LOGLEVEL_AUDIOKIT = AudioKitInfo;
    Serial.begin(115200);
    // open in write mode
    AudioKitConfig cfg = kit.defaultConfig(AudioOutput);
    kit.begin(cfg);
    app.start();

    for (uint8_t p = 0; p < PIN_KEYS_COUNT; p++) {
        pinMode(pinKeys[p], INPUT_PULLUP);
    }

    initDisplay();
    Serial.println("Start esp32 zic server");
}

void zicServerEsp32Loop()
{
    static union sampleTUNT {
        uint32_t sample;
        int16_t ch[2];
    } sampleDataU;

    for (int i = 0; i < 512; ++i) {
        sampleDataU.ch[0] = app.sample() * 3;
        sampleDataU.ch[1] = sampleDataU.ch[0];
        kit.write((const char*)&sampleDataU.sample, 4);
    }

    buttonLoop();
}

#endif
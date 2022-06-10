// TODO
// Fix audio
// maybe merge SDL1.2 and 2 into one or at least common part

#include <SDL/SDL.h>
#include <stdio.h>

#include "../app/app.h"
#include "../app/app_def.h"
#include "../app/app_display.h"
#include "font.h"

#ifndef SAMPLE_RATE
#define SAMPLE_RATE 44100
#endif

#define CHANNELS 1
//  #define CHANNELS 2 // to be fixed

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define TEXT_SIZE 2

// LAPTOP
#define KEY_UP 111
#define KEY_DOWN 116
#define KEY_LEFT 113
#define KEY_RIGHT 114
#define KEY_A 38
#define KEY_B 39
#define KEY_MENU 9

// MIYOO
// #define KEY_UP 103
// #define KEY_DOWN 108
// #define KEY_LEFT 105
// #define KEY_RIGHT 106
// #define KEY_A 57
// #define KEY_B 29
// #define KEY_MENU 1

App app;

typedef struct {
    bool keysChanged = false;
    Uint8 keys = 0;
} Ui;

Ui ui;

bool handleKeyboard(SDL_KeyboardEvent* event)
{
    uint8_t bit;

    printf("handleKeyboard %d\n", event->keysym.scancode);
    // printf("handleKeyboard %d\n", event->repeat);
    switch (event->keysym.scancode) {
    case KEY_UP:
        bit = UI_KEY_UP;
        break;
    case KEY_DOWN:
        bit = UI_KEY_DOWN;
        break;
    case KEY_LEFT:
        bit = UI_KEY_LEFT;
        break;
    case KEY_RIGHT:
        bit = UI_KEY_RIGHT;
        break;
    case KEY_A:
        bit = UI_KEY_A;
        break;
    case KEY_B:
        bit = UI_KEY_B;
        break;
    case KEY_MENU:
        return false;
    default:
        return true;
    }

    if (event->type == SDL_KEYDOWN) {
        ui.keys |= 1 << bit; // Set bit to 1
    } else {
        ui.keys &= ~(1 << bit); // unset bit (to 0)
    }

    // we could skip keyChange on A repeat
    ui.keysChanged = true;
    return true;
}

bool handleEvent()
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            return false;

        case SDL_KEYUP:
        case SDL_KEYDOWN:
            return handleKeyboard(&event.key);
        // could be useful to simulate a pot
        // case SDL_MOUSEWHEEL:
        //     break;
        case SDL_JOYAXISMOTION:
            printf("SDL_JOYAXISMOTION");
            break;
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            printf("SDL_JOYBUTTON");
            break;
        }
    }

    return true;
}

void audioCallBack(void* userdata, Uint8* stream, int len)
{
    static union sampleTUNT {
        Uint8 ch[2];
        int16_t sample;
    } sampleDataU;

    for (int i = 0; i < len; i++) {
        sampleDataU.sample = app.sample();
        stream[i] = sampleDataU.ch[0];
        i++;
        stream[i] = sampleDataU.ch[1];

#if CHANNELS == 2
        i++;
        buffer[i] = sampleDataU.ch[0];
        i++;
        buffer[i] = sampleDataU.ch[1];
#endif
    }
}

bool initAudio()
{
    SDL_AudioSpec spec, aspec;

    spec.freq = SAMPLE_RATE;
    spec.format = AUDIO_S16;
    spec.channels = CHANNELS;
    spec.samples = 2048;
    spec.callback = audioCallBack;
    spec.userdata = NULL;

    if (SDL_OpenAudio(&spec, &aspec) <= 0) {
        fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
        return false;
    }

    printf("aspec freq %d channel %d sample %d format %d", aspec.freq, aspec.channels, aspec.samples, aspec.format);

    // // Start playing, "unpause"
    // SDL_PauseAudioDevice(ui.audioDevice, 0);
    SDL_PauseAudio(0);

    return true;
}

void render(SDL_Surface* screenSurface, App_Display* display)
{
    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));
    draw_string(screenSurface, display, 2, TEXT_SIZE * FONT_H, TEXT_SIZE);
    SDL_Flip(screenSurface);
}

int main(int argc, char* args[])
{

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "could not initialize sdl: %s\n", SDL_GetError());
        return 1;
    }

    // if (!initAudio()) {
    //     return 1;
    // }

    SDL_Surface* screenSurface = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_SWSURFACE);
    if (screenSurface == 0) {
        fprintf(stderr, "Failed to set video mode\n");
        return 1;
    }

    init_default_string_color(screenSurface);

    app.start();
    render(screenSurface, app.render());

    while (handleEvent()) {
        if (ui.keysChanged) {
            ui.keysChanged = false;
            render(screenSurface, app.handleUi(ui.keys));
        }
        // SDL_Delay(10);
    }

    // SDL_CloseAudioDevice(ui.audioDevice);
    SDL_CloseAudio();

    SDL_Quit();
    return 0;
}
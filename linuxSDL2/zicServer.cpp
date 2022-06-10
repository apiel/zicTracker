#include <SDL2/SDL.h>
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

#if ZIC_TARGET == 1
// OPENDINGUX
#define KEY_UP SDL_SCANCODE_UP
#define KEY_DOWN SDL_SCANCODE_DOWN
#define KEY_LEFT SDL_SCANCODE_LEFT
#define KEY_RIGHT SDL_SCANCODE_RIGHT
#define KEY_A 224
#define KEY_B 226
#define KEY_MENU 40
#else
// LAPTOP
#define KEY_UP SDL_SCANCODE_UP
#define KEY_DOWN SDL_SCANCODE_DOWN
#define KEY_LEFT SDL_SCANCODE_LEFT
#define KEY_RIGHT SDL_SCANCODE_RIGHT
#define KEY_A SDL_SCANCODE_S
#define KEY_B SDL_SCANCODE_A
#define KEY_MENU SDL_SCANCODE_ESCAPE
#endif

App app;

typedef struct {
    bool keysChanged = false;
    Uint8 keys = 0;
} Ui;

Ui ui;

bool handleKeyboard(SDL_KeyboardEvent* event)
{
    uint8_t bit;

    // SDL_Log("handleKeyboard %d", event->keysym.scancode);
    // SDL_Log("handleKeyboard %d", event->repeat);
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
    case KEY_B: // Should be configurable!
        bit = UI_KEY_B;
        break;
    case KEY_MENU:
        return false;
    default:
        break;
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
            SDL_Log("SDL_JOYAXISMOTION");
            break;
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            SDL_Log("SDL_JOYBUTTON");
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

SDL_AudioDeviceID initAudio()
{
    SDL_AudioSpec spec, aspec;

    SDL_zero(spec);
    spec.freq = SAMPLE_RATE;
    spec.format = AUDIO_S16;
    spec.channels = CHANNELS;
    // spec.samples = 128;
    spec.samples = 2048;
    spec.callback = audioCallBack;
    spec.userdata = NULL;

    SDL_AudioDeviceID audioDevice = SDL_OpenAudioDevice(NULL, 0, &spec, &aspec, SDL_AUDIO_ALLOW_ANY_CHANGE);
    if (audioDevice <= 0) {
        fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
        return false;
    }

    SDL_Log("aspec freq %d channel %d sample %d format %d", aspec.freq, aspec.channels, aspec.samples, aspec.format);

    // Start playing, "unpause"
    SDL_PauseAudioDevice(audioDevice, 0);

    return audioDevice;
}

void render(SDL_Window* window, SDL_Surface* screenSurface, App_Display* display)
{
    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));
    SDL_Log("\n%s\n", display->text);
    draw_string(screenSurface, display, 2, TEXT_SIZE * FONT_H, TEXT_SIZE);
    SDL_UpdateWindowSurface(window);
}

int main(int argc, char* args[])
{
    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
        return 1;
    }

    SDL_AudioDeviceID audioDevice = initAudio();
    if (!audioDevice) {
        return 1;
    }

    window = SDL_CreateWindow(
        "Zic",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);

    if (window == NULL) {
        fprintf(stderr, "could not create window: %s\n", SDL_GetError());
        return 1;
    }
    screenSurface = SDL_GetWindowSurface(window);
    init_default_string_color(screenSurface);

    // SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

    // draw_string(screenSurface, "abcdefghijklmnopqrstu\nabcdefghijklmnopqrstu\nabcdefghijklmnopqrstu\nabcdefghijklmnopqrstu\nabcdefghijklmnopqrstu\nabcdefghijklmnopqrstu\nabcdefghijklmnopqrstu\nabcdefghijklmnopqrstu\n", 2, TEXT_SIZE * FONT_H, TEXT_SIZE, '0');
    // SDL_UpdateWindowSurface(window);
    // while (handleEvent()) {
    //     SDL_Delay(10);
    // }

    // draw_string(screenSurface, "Zic", 2, 20, 5);

    SDL_UpdateWindowSurface(window);

    app.start();
    render(window, screenSurface, app.render());

    while (handleEvent()) {
        if (ui.keysChanged) {
            ui.keysChanged = false;
            render(window, screenSurface, app.handleUi(ui.keys));
        }
        // SDL_Delay(10);
    }

    SDL_CloseAudioDevice(audioDevice);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
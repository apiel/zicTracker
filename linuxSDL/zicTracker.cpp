#include <SDL2/SDL.h>

#if ZIC_SDL_MIXER
#include <SDL2/SDL_mixer.h>
#else
#define __LINUX_PULSE__
#endif

#include <stdio.h>

#define SAMPLE_RATE 44100
// #define CHANNELS 1
#define CHANNELS 2 // to be fixed
#define APP_AUDIO_FORMAT AUDIO_S16LSB
#define APP_AUDIO_CHUNK 4096

// THIS MUST BE ON TOP
// this include must be before initilizing zic lib, to be sure to override zic_file.h!
#include "zic_file_sdl.h"

#include "display.h"

#include "../app/app.h"
#include "../app/app_def.h"
#include "../app/app_display.h"
#include "../app/app_file.h"
#include "../app/app_patterns.h"
#include "zicKeyMap.h"

UI_Display display;
App_Patterns patterns;
App app(&patterns, &display);

typedef struct {
    bool keysChanged = false;
    Uint8 keys = 0;
} Ui;

Ui ui;

bool handleKeyboard(SDL_KeyboardEvent* event)
{
    uint8_t bit;

    SDL_Log("handleKeyboard %d\n", event->keysym.scancode);
    // SDL_Log("handleKeyboard %d\n", event->repeat);
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
        bit = UI_KEY_EDIT;
        break;
    case KEY_B: // Should be configurable!
        bit = UI_KEY_MENU;
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

bool handleController(SDL_ControllerButtonEvent* event)
{
    uint8_t bit;

    SDL_Log("handleController btn %d state %d\n", event->button, event->state);
    switch (event->button) {
    case 11:
        bit = UI_KEY_UP;
        break;
    case 12:
        bit = UI_KEY_DOWN;
        break;
    case 13:
        bit = UI_KEY_LEFT;
        break;
    case 14:
        bit = UI_KEY_RIGHT;
        break;
    case 0:
        bit = UI_KEY_EDIT;
        break;
    case 1: // Should be configurable!
        bit = UI_KEY_MENU;
        break;
    case 9:
        return false;
    default:
        break;
    }

    if (event->state == SDL_PRESSED) {
        ui.keys |= 1 << bit; // Set bit to 1
    } else {
        ui.keys &= ~(1 << bit); // unset bit (to 0)
    }

    // we could skip keyChange on A repeat
    ui.keysChanged = true;

    return true;
}

void initController()
{
    int num = SDL_NumJoysticks();
    // SDL_Log("%d joysticks connected\n", num);
    for (int i = 0; i < num; i++) {
        SDL_JoystickOpen(i);
        // SDL_Joystick* joystick = SDL_JoystickOpen(i);
        // SDL_Log("Joystick %d: %s\n", i, SDL_JoystickName(joystick));
        if (SDL_IsGameController(i)) {
            SDL_GameControllerOpen(i);
            // SDL_GameController* controller = SDL_GameControllerOpen(i);
            // SDL_Log("Game controller %d: %s\n", i, SDL_GameControllerName(controller));
        }
    }
}

bool handleEvent()
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        // if (event.type > 0x300 && event.type < 0x800) {
        //     SDL_Log("handleEvent %d\n", event.type);
        // }

        switch (event.type) {
        case SDL_QUIT:
            return false;

        case SDL_KEYUP:
        case SDL_KEYDOWN:
            return handleKeyboard(&event.key);
            // could be useful to simulate a pot
            // case SDL_MOUSEWHEEL:
            //     break;

        // case SDL_JOYDEVICEADDED:
        //     SDL_Log("handleEvent SDL_JOYDEVICEADDED\n");
        //     break;
        case SDL_JOYAXISMOTION:
            SDL_Log("SDL_JOYAXISMOTION\n");
            break;
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            SDL_Log("SDL_JOYBUTTON\n");

        case SDL_CONTROLLERDEVICEADDED:
            // SDL_Log("SDL_CONTROLLERDEVICEADDED\n");
            initController();
            break;
        // case SDL_CONTROLLERAXISMOTION:
        //     SDL_Log("SDL_CONTROLLERAXISMOTION\n");
        //     break;
        case SDL_CONTROLLERBUTTONDOWN:
        case SDL_CONTROLLERBUTTONUP:
            return handleController(&event.cbutton);
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
        stream[i] = sampleDataU.ch[0];
        i++;
        stream[i] = sampleDataU.ch[1];
#endif
    }
}

#ifndef ZIC_SDL_MIXER
SDL_AudioDeviceID initAudio()
{
    SDL_AudioSpec spec, aspec;

    SDL_zero(spec);
    spec.freq = SAMPLE_RATE;
    spec.format = APP_AUDIO_FORMAT;
    spec.channels = CHANNELS;
    spec.samples = APP_AUDIO_CHUNK;
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
#endif

int main(int argc, char* args[])
{
    SDL_Log(">>>>>>> Start Zic Tracker\n");
    // // patterns.debug(SDL_Log);
    // patterns.debug(SDL_Log, 1);
    patterns.debug(SDL_Log, 6);
    // patterns.debug(SDL_Log, 5);
    // patterns.debug(SDL_Log, 199);

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

#if ZIC_SDL_MIXER
    if (Mix_OpenAudio(SAMPLE_RATE, APP_AUDIO_FORMAT, CHANNELS, APP_AUDIO_CHUNK) < 0) {
        fprintf(stderr, "Open audio mixer error: %s\n", SDL_GetError());
    } else {
        SDL_Log("Open audio mixer success\n");
    }
    Mix_HookMusic(audioCallBack, NULL);
#else
    SDL_AudioDeviceID audioDevice = initAudio();
    if (SDL_getenv("ZIC_SKIP_AUDIO") == NULL && !audioDevice) {
        return 1;
    }
#endif

    char fileFound[256];
    nextFile(fileFound, "samples", "kick.wav", -1);
    SDL_Log("Next file: %s\n", fileFound);

    SDL_Window* window = SDL_CreateWindow(
        "Zic",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_W, SCREEN_H,
        SDL_WINDOW_SHOWN);

    if (window == NULL) {
        fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Surface* screenSurface = SDL_GetWindowSurface(window);

    display.init(screenSurface);
    app.start();
    app.render();
    SDL_UpdateWindowSurface(window);

    while (handleEvent()) {
        if (ui.keysChanged) {
            ui.keysChanged = false;
            app.handleUi(ui.keys);
            SDL_Log("\n%s\n", display.text);
            // SDL_UpdateWindowSurface(window);
        }
        // SDL_Delay(10);
        if (app.rendered) {
            app.rendered = false;
            SDL_UpdateWindowSurface(window);
        }
    }

    SDL_DestroyWindow(window);

#if ZIC_SDL_MIXER
    Mix_CloseAudio();
#else
    SDL_CloseAudioDevice(audioDevice);
#endif

    SDL_Quit();
    return 0;
}
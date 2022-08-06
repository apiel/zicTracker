#include <SDL2/SDL.h>
// TODO make SDL mixer optional
#include <SDL2/SDL_mixer.h>
// #define __LINUX_PULSE__

#include <stdio.h>

#define SAMPLE_RATE 48000
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
            SDL_Log("SDL_JOYAXISMOTION\n");
            break;
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            SDL_Log("SDL_JOYBUTTON\n");
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
        stream[i] = sampleDataU.ch[0];
        i++;
        stream[i] = sampleDataU.ch[1];
#endif
    }
}

// void render(SDL_Surface* screenSurface, App_Display* display)
// {
//     SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, UI_COLOR_BG));
//     // SDL_Log("Cursor: %d(%d)\n", display->cursorPos, display->cursorLen);
//     SDL_Log("\n%s\n", display->text);
//     draw_string(screenSurface, display, 2, TEXT_SIZE * FONT_H, TEXT_SIZE);
// }

int main(int argc, char* args[])
{
    SDL_Log(">>>>>>> Start Zic Tracker\n");
    // // patterns.debug(SDL_Log);
    // patterns.debug(SDL_Log, 1);
    patterns.debug(SDL_Log, 6);
    // patterns.debug(SDL_Log, 5);
    // patterns.debug(SDL_Log, 199);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    if (Mix_OpenAudio(SAMPLE_RATE, APP_AUDIO_FORMAT, CHANNELS, APP_AUDIO_CHUNK) < 0) {
        fprintf(stderr, "Open audio mixer error: %s\n", SDL_GetError());
    } else {
        SDL_Log("Open audio mixer success\n");
    }
    Mix_HookMusic(audioCallBack, NULL);

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
            SDL_UpdateWindowSurface(window);
        }
        // SDL_Delay(10);
    }

    SDL_DestroyWindow(window);

    Mix_CloseAudio();
    SDL_Quit();
    return 0;
}
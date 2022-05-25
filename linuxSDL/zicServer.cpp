#include <SDL2/SDL.h>
#include <stdio.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

typedef struct {
    SDL_Window* window;
    bool keyEvent = false;
    bool keyUp = false;
    bool keyDown = false;
    bool keyRight = false;
    bool keyLeft = false;
    bool keyA = false;
    bool keyB = false;
    bool keyX = false;
    bool keyY = false;
} Ui;

Ui ui;

void handleKeyboard(SDL_KeyboardEvent* event)
{
    // SDL_Log("handleKeyboard %d", event->repeat);

    switch (event->keysym.scancode) {
    case SDL_SCANCODE_UP:
        ui.keyUp = event->type == SDL_KEYDOWN;
        break;
    case SDL_SCANCODE_DOWN:
        ui.keyDown = event->type == SDL_KEYDOWN;
        break;
    case SDL_SCANCODE_RIGHT:
        ui.keyRight = event->type == SDL_KEYDOWN;
        break;
    case SDL_SCANCODE_LEFT:
        ui.keyLeft = event->type == SDL_KEYDOWN;
        break;
    case SDL_SCANCODE_A:
        ui.keyA = event->type == SDL_KEYDOWN; // && event->repeat == 0;
        break;
    case SDL_SCANCODE_S: // TODO Those should be configurable
        ui.keyB = event->type == SDL_KEYDOWN; // && event->repeat == 0;
        break;
    case SDL_SCANCODE_Z:
        ui.keyY = event->type == SDL_KEYDOWN; // && event->repeat == 0;
        break;
    case SDL_SCANCODE_X:
        ui.keyX = event->type == SDL_KEYDOWN; // && event->repeat == 0;
        break;
    default:
        break;
    }

    ui.keyEvent = true;
    // ui.keyEvent = ui.keyUp || ui.keyDown || ui.keyLeft || ui.keyRight || ui.keyA || ui.keyB || ui.keyY || ui.keyX;
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
            handleKeyboard(&event.key);
            break;
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

int main(int argc, char* args[])
{
    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
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
    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
    SDL_UpdateWindowSurface(window);

    while (handleEvent()) {
        if (ui.keyEvent) {
            SDL_Log("key pressed Up %d Down %d Left %d Right %d A %d B %d Y %d X %d",
                ui.keyUp, ui.keyDown, ui.keyLeft, ui.keyRight, ui.keyA, ui.keyB, ui.keyY, ui.keyX);
            ui.keyEvent = false;
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
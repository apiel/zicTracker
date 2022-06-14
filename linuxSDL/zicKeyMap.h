#ifndef ZIC_KEY_MAP_H_
#define ZIC_KEY_MAP_H_

#if ZIC_SDL2
#include <SDL2/SDL.h>
#define __LINUX_PULSE__
#else
#include <SDL/SDL.h>
#endif

// might use SDL_getenv("ZIC_SKIP_AUDIO") or config file instead
#if ZIC_SDL2
#if ZIC_TARGET == 1
// OPENDINGUX
#define KEY_UP SDL_SCANCODE_UP
#define KEY_DOWN SDL_SCANCODE_DOWN
#define KEY_LEFT SDL_SCANCODE_LEFT
#define KEY_RIGHT SDL_SCANCODE_RIGHT
#define KEY_A 224
#define KEY_B 226
#define KEY_MENU 40
#elseif ZIC_TARGET == 2
// MIYOO
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
#else
// SDL1.2
// MIYOO
#define KEY_UP 103
#define KEY_DOWN 108
#define KEY_LEFT 105
#define KEY_RIGHT 106
#define KEY_A 57
#define KEY_B 29
#define KEY_MENU 1
#endif

#endif
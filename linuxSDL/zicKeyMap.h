#ifndef ZIC_KEY_MAP_H_
#define ZIC_KEY_MAP_H_

// might use SDL_getenv("ZIC_SKIP_AUDIO") or config file instead
#if ZIC_SDL2
#include "./zicKeyMapSDL2.h"
#else
#include "./zicKeyMapSDL1.2.h"
#endif

#endif
// Keep this ifndef naming to get priority over zic_file.h.
#ifndef ZIC_FILE_H_
#define ZIC_FILE_H_

#include <stdint.h>

#if ZIC_SDL2
#include <SDL2/SDL.h>
#else
#include <SDL/SDL.h>
#endif

void* zic_file_open(const char* filename, const char *mode)
{
    return SDL_RWFromFile(filename, mode);
}

bool zic_file_read(void* file, void* ptr, uint16_t size)
{
    return SDL_RWread((SDL_RWops *)file, ptr, size, 1) > 0;
}

bool zic_file_seek(void* file, uint64_t offset)
{
    return SDL_RWseek((SDL_RWops *)file, offset, SEEK_SET) != -1;
}

bool zic_file_close(void* file)
{
    return SDL_RWclose((SDL_RWops *)file) == 0;
}

#endif

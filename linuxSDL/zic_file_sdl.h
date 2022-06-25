// Keep this ifndef naming to get priority over zic_file.h.
#ifndef ZIC_FILE_H_
#define ZIC_FILE_H_

#include <stdint.h>

#if ZIC_SDL2
#include <SDL2/SDL.h>
#else
#include <SDL/SDL.h>
#endif

class Zic_File {
public:
    void* file;

    Zic_File()
    {
    }

    Zic_File(const char* filename, const char* mode)
    {
        open(filename, mode);
    }

    ~Zic_File()
    {
        close();
    }

    void* open(const char* filename, const char* mode)
    {
        file = SDL_RWFromFile(filename, mode);
        return file;
    }

    bool read(void* ptr, uint16_t size)
    {
        return SDL_RWread((SDL_RWops*)file, ptr, size, 1) > 0;
    }

    bool seek(uint64_t offset)
    {
        return SDL_RWseek((SDL_RWops*)file, offset, SEEK_SET) != -1;
    }

    bool close()
    {
        return SDL_RWclose((SDL_RWops*)file) == 0;
    }
};

#endif

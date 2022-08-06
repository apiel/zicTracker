// Keep this ifndef naming to get priority over zic_file.h.
#ifndef ZIC_FILE_H_
#define ZIC_FILE_H_

#include <stdint.h>

#include <SDL2/SDL.h>

class Zic_File {
public:
    void* file = NULL;

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

    virtual void* open(const char* filename, const char* mode)
    {
        file = SDL_RWFromFile(filename, mode);
        return file;
    }

    virtual bool isOpen()
    {
        return file != NULL;
    }

    virtual uint64_t read(void* ptr, uint16_t size)
    {
        return (uint64_t)SDL_RWread((SDL_RWops*)file, ptr, size, 1);
    }

    virtual uint64_t write(void* ptr, uint16_t size)
    {
        return (uint64_t)SDL_RWwrite((SDL_RWops*)file, ptr, size, 1);
    }

    virtual bool seekFromStart(uint64_t offset)
    {
        return SDL_RWseek((SDL_RWops*)file, offset, SEEK_SET) != -1;
    }

    virtual bool seekFromCurrent(uint64_t offset)
    {
        return SDL_RWseek((SDL_RWops*)file, offset, SEEK_CUR) != -1;
    }

    virtual bool seekFromEnd(uint64_t offset)
    {
        return SDL_RWseek((SDL_RWops*)file, offset, SEEK_END) != -1;
    }

    virtual uint64_t tell()
    {
        return SDL_RWtell((SDL_RWops*)file);
    }

    virtual bool close()
    {
        if (file == NULL) {
            return true;
        }
        if (SDL_RWclose((SDL_RWops*)file) == 0) {
            file = NULL;
            return true;
        }
        return false;
    }
};

#endif

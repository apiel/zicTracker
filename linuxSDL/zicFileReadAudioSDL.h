#ifndef ZIC_FILE_READ_AUDIO_SDL_H_
#define ZIC_FILE_READ_AUDIO_SDL_H_

#include <zic_file_readAudio.h>

#if ZIC_SDL2
#include <SDL2/SDL.h>
#else
#include <SDL/SDL.h>
#endif

class Zic_File_ReadAudioSDL : public Zic_File_ReadAudio {
protected:
    SDL_RWops* file;

public:
    bool open(char* filename)
    {
        file = SDL_RWFromFile(filename, "rb");

        return file != NULL;
    }

    bool read(void* ptr, uint16_t size)
    {
        return SDL_RWread(file, ptr, size, 1) > 0;
    }

    bool seek(uint64_t offset)
    {
        return SDL_RWseek(file, offset, SEEK_SET) != -1;
    }

    bool close()
    {
        return SDL_RWclose(file) == 0;
    }
};

#endif

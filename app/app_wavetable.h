#ifndef APP_WAVETABLE_H_
#define APP_WAVETABLE_H_

#include <app_core_file.h>
// #include <zic_wave_file.h>
#include <zic_wavetable_file.h>

#define APP_WAVETABLES_FOLDER "wavetables"

// class App_Wavetable : public Zic_Wave_File {
class App_Wavetable : public Zic_Wavetable_File {
protected:
    void openCurrentFilename()
    {
        char filepath[270];
        snprintf(filepath, 270, "%s/%s", APP_WAVETABLES_FOLDER, filename);
        open(filepath);
        // printf("App_Wavetable::openCurrentFilename: %s\n", filepath);
    }

public:
    char filename[256] = "";

    App_Wavetable()
    {
        firstFile(filename, APP_WAVETABLES_FOLDER);
        openCurrentFilename();
    }

    App_Wavetable* setNext(int8_t direction = 0)
    {
        // TODO if direction > 1 then should jump to next letter
        direction = range(direction, -1, 1);
        nextFile(filename, APP_WAVETABLES_FOLDER, filename, direction);
        openCurrentFilename();
        return this;
    }
};

#endif
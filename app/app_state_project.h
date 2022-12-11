#ifndef APP_STATE_PROJECT_H_
#define APP_STATE_PROJECT_H_

#include <app_core_file.h>
#include <stdint.h>

#include "./app_state_base.h"
#include "./app_tempo.h"

#define PROJECT_NAME_LEN 22

class App_State_Project : public App_State_Base {
protected:
    // Do not change order or remove state keys!!
    enum {
        APP_STATE_PROJECT_NAME = 0,
        APP_STATE_TEMPO,
    };

public:
    uint16_t id;
    char name[PROJECT_NAME_LEN];
    const char* statePath = "projects/current/project.zic";

    void save()
    {
        APP_LOG("Save state %s\n", statePath);

        Zic_File file(statePath, "w");
        if (!file.isOpen()) {
            APP_LOG("Could not open file %s\n", statePath);
            return;
        }

        write(&file, APP_STRING, APP_STATE_PROJECT_NAME, name);
        write(&file, APP_NUMBER, APP_STATE_TEMPO, &App_Tempo::getInstance()->bpm);

        file.close();
    }

    void load()
    {
        APP_LOG("Load state %s\n", statePath);

        Zic_File file(statePath, "r");
        if (!file.isOpen()) {
            APP_LOG("Could not open file %s\n", statePath);
            return;
        }

        char buffer[APP_STATE_BUFFER];
        while (file.read(buffer, APP_STATE_BUFFER) > 0) {
            char key[4];
            memcpy(key, buffer, 3);
            key[3] = '\0';

            switch (atoi(key)) {
            case APP_STATE_PROJECT_NAME:
                memcpy(name, buffer + 4, sizeof(name) - 1);
                // removeTrailingSpaces(name);
                break;

            case APP_STATE_TEMPO:
                App_Tempo::getInstance()->bpm = atoi(buffer + 4);
                break;

            default:
                break;
            }
        }

        file.close();
    }
};

#endif
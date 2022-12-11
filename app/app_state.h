#ifndef APP_STATE_H_
#define APP_STATE_H_

#include "./app_def.h"
#include <zic_seq_pattern.h>

class App_State {
    protected:
        App_State() {}

public:
    static App_State* instance;

    static App_State* getInstance()
    {
        if (!instance) {
            instance = new App_State();
        }
        return instance;
    }

    Zic_Seq_Pattern patterns[PATTERN_COUNT];
};

App_State* App_State::instance = NULL;

#endif
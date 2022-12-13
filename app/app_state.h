#ifndef APP_STATE_H_
#define APP_STATE_H_

#include "./app_def.h"
#include "./app_state_project.h"
#include "./app_state_pattern.h"

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

    App_State_Project project;
    App_State_Pattern pattern;

    void save() {
        project.save();
        pattern.save();
    }

    void load() {
        project.load();
        pattern.load();
    }
};

App_State* App_State::instance = NULL;

#endif
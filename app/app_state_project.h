#ifndef APP_STATE_PROJECT_H_
#define APP_STATE_PROJECT_H_

#include <stdint.h>
#include <app_core_file.h>

#define PROJECT_NAME_LEN 22

class App_State_Project {
public:
    uint16_t id;
    char name[PROJECT_NAME_LEN];
};

#endif
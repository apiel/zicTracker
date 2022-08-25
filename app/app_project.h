#ifndef APP_PROJECT_H_
#define APP_PROJECT_H_

#include <stdio.h>
#include <string.h>

#include "./app_def.h"
#include "./app_file.h"

#define STEP_DATA_LEN 9
#define PATTERN_DATA_LEN STEP_DATA_LEN * MAX_STEPS_IN_PATTERN
#define SAME_INSTRUMENT_SYMBOL '_'

#define PROJECT_FILE_FORMAT "projects/%d/project.config", project
#define CURRENT_PROJECT_FILE_FORMAT "projects/current_project.config", project

class App_Project {
protected:
    uint8_t project = 0;

public:

    App_Project()
    {

    }

    void load(uint8_t pos)
    {
    }

    void save(uint8_t pos)
    {
    }
};

#endif
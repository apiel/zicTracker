#ifndef APP_PROJECT_H_
#define APP_PROJECT_H_

#include <stdio.h>
#include <string.h>

#include "./app_def.h"
#include "./app_file.h"

#define PROJECT_NAME_LEN 22
#define PROJECT_FILE_FORMAT "projects/%d/project.config", project
#define CURRENT_PROJECT_FILE_FORMAT "projects/current_project.config"

class App_Project {
protected:
    typedef struct Project {
        uint16_t id;
        char name[PROJECT_NAME_LEN];
        uint8_t bpm;

        // Sequencer
        // Tracks settings
        // Instrument settings

    } Project;

public:
    Project project;

    App_Project()
    {
        load();
    }

    void load()
    {
        if (!loadFileContent((char* )&project, sizeof(Project), CURRENT_PROJECT_FILE_FORMAT)) {
            project.id = 0;
            strncpy(project.name, "New project", PROJECT_NAME_LEN);
            project.bpm = 120;
        }
        // printf("Loaded project %d: %s (bpm %d)\n", project.id, project.name, project.bpm);
    }

    void autoSave()
    {
    }
};

#endif
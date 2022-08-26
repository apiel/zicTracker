#ifndef APP_PROJECT_H_
#define APP_PROJECT_H_

#include <stdio.h>
#include <string.h>

#include "./app_def.h"
#include "./app_file.h"
#include "./app_tracks.h"

#include <zic_seq_tempo.h>

#define PROJECT_NAME_LEN 22
#define PROJECT_FILE_FORMAT "projects/%d/project.config", project
#define CURRENT_PROJECT_FILE_FORMAT "projects/current_project.config"

class App_Project {
protected:
    App_Tracks * tracks;
    Zic_Seq_Tempo<> * tempo;

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

    App_Project(App_Tracks* _tracks, Zic_Seq_Tempo<>* _tempo)
    : tracks(_tracks)
    , tempo(_tempo)
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
        tempo->set(project.bpm);
        // printf("Loaded project %d: %s (bpm %d)\n", project.id, project.name, project.bpm);
    }

    void autoSave()
    {
        project.bpm = tempo->getBpm();
        saveFileContent((char* )&project, sizeof(Project), CURRENT_PROJECT_FILE_FORMAT);
    }
};

#endif
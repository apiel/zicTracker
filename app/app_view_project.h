#ifndef APP_VIEW_PROJECT_H_
#define APP_VIEW_PROJECT_H_

#include "./app_state.h"
#include "./app_state_project.h"
#include "./app_tempo.h"
#include "./app_view_menu.h"
#include <app_core_renderer.h>
#include <app_core_util.h>
#include <app_core_view_table.h>

#define VIEW_PROJECT_ROW 3
#define VIEW_PROJECT_COL 3

class App_View_ProjectBpm : public App_View_TableLabeledRow {
protected:
    App_Tempo* tempo;

public:
    App_View_ProjectBpm()
        : App_View_TableLabeledRow("BPM  ", 3)
    {
        tempo = App_Tempo::getInstance();
    }

    void renderValue(App_Renderer* renderer, uint8_t col)
    {
        sprintf(renderer->text + strlen(renderer->text), "%-3d", tempo->getBpm());
    }

    uint8_t update(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col) override
    {
        if (keys->Right) {
            tempo->set(tempo->getBpm() + 1);
        } else if (keys->Up) {
            tempo->set(tempo->getBpm() + 10);
        } else if (keys->Left) {
            tempo->set(tempo->getBpm() - 1);
        } else if (keys->Down) {
            tempo->set(tempo->getBpm() - 10);
        }
        return VIEW_CHANGED;
    }
};

class App_View_ProjectPlay : public App_View_TableLabeledRow {
public:
    App_View_ProjectPlay()
        : App_View_TableLabeledRow("SEQ  ", 5)
    {
    }

    void renderValue(App_Renderer* renderer, uint8_t col)
    {
        if (col == 1) {
            if (App_Tracks::getInstance()->isPlaying()) {
                strcat(renderer->text, ">PLAY ");
            } else {
                strcat(renderer->text, "PAUSE ");
            }
        } else {
            strcat(renderer->text, "RESET");
        }
    }

    uint8_t update(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col) override
    {
        if (col == 1) {
            App_Tracks::getInstance()->togglePlay();
        } else {
            App_Tracks::getInstance()->reset();
        }
        return VIEW_CHANGED;
    }
};

class App_View_ProjectName : public App_View_TableLabeledRow {
protected:
    App_View_Menu* menu;

public:
    App_View_ProjectName(App_View_Menu* _menu)
        : App_View_TableLabeledRow("Name ", PROJECT_NAME_LEN)
        , menu(_menu)
    {
    }

    void renderValue(App_Renderer* renderer, uint8_t col)
    {
        sprintf(renderer->text + strlen(renderer->text), "%-*s", PROJECT_NAME_LEN, App_State::getInstance()->project.name);
    }

    uint8_t update(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col) override
    {
        menu->setView(41);
        return VIEW_CHANGED;
    }
};

class App_View_Project : public App_View_Table {
protected:
    App_View_ProjectBpm bpmField;
    App_View_ProjectPlay playField;
    App_View_ProjectName nameField;

    App_View_TableField* fields[VIEW_PROJECT_ROW * VIEW_PROJECT_COL] = {
        // clang-format off
        &nameField, &nameField, NULL,
        &bpmField, &bpmField, NULL,
        &playField, &playField, &playField,
        // clang-format on
    };

    App_View_Project(App_View_Menu* menu)
        : App_View_Table(fields, VIEW_PROJECT_ROW, VIEW_PROJECT_COL)
        , nameField(menu)
    {
        initSelection();
    }

public:
    static App_View_Project* instance;

    static App_View_Project* getInstance(App_View_Menu* menu)
    {
        if (!instance) {
            instance = new App_View_Project(menu);
        }
        return instance;
    }

    void preRender(App_Renderer* renderer)
    {
        renderer->useColor(0, 255, 0, 4, COLOR_MEDIUM);
        App_View_Table::preRender(renderer);
    }

    const char* snapshotPath = "projects/current/project.zic";

    // void snapshot(App_Renderer* renderer) override
    // {
    //     render(renderer);
    //     // printf("Saving snapshot:\n%s", renderer->text);
    //     saveFileContent(renderer->text, strlen(renderer->text), snapshotPath);
    // }

    // void loadSnapshot() override
    // {
    //     Zic_File file(snapshotPath, "r");
    //     if (file.isOpen()) {
    //         file.seekFromStart(5);
    //         file.read(App_State::getInstance()->project.name, PROJECT_NAME_LEN);
    //         App_State::getInstance()->project.name[PROJECT_NAME_LEN - 1] = '\0';
    //         file.seekFromCurrent(6);
    //         char bpm[3];
    //         file.read(bpm, 3);
    //         // printf("yoyoyo BPM: %s\n", bpm);
    //         trimToNumeric(bpm);
    //         App_Tempo::getInstance()->set(atoi(bpm));

    //         file.close();
    //     }
    // }
};

App_View_Project* App_View_Project::instance = NULL;

#endif
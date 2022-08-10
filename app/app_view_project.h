#ifndef APP_VIEW_PROJECT_H_
#define APP_VIEW_PROJECT_H_

#include <zic_seq_tempo.h>

#include "./app_display.h"
#include "./app_view_table.h"

#define VIEW_PROJECT_ROW 2
#define VIEW_PROJECT_COL 2

class App_View_ProjectBpm : public App_View_TableLabeledRow {
protected:
    Zic_Seq_Tempo<>* tempo;

public:
    App_View_ProjectBpm(Zic_Seq_Tempo<>* _tempo)
        : App_View_TableLabeledRow("BPM ", 3)
        , tempo(_tempo)
    {
    }

    void renderValue(App_Display* display, uint8_t col)
    {
        sprintf(display->text + strlen(display->text), "%-3d", tempo->getBpm());
    }

    uint8_t update(UiKeys* keys, App_Display* display, uint8_t row, uint8_t col) override
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

class App_View_Project : public App_View_Table {
protected:
    App_View_ProjectBpm bpmField;

    App_View_TableField* fields[VIEW_PROJECT_ROW * VIEW_PROJECT_COL] = {
        // clang-format off
        &bpmField, &bpmField,
        NULL, NULL,
        // clang-format on
    };

public:
    App_View_Project(Zic_Seq_Tempo<>* tempo)
        : App_View_Table(fields, VIEW_PROJECT_ROW, VIEW_PROJECT_COL)
        , bpmField(tempo)
    {
        initSelection();
    }

    void initDisplay(App_Display* display)
    {
        display->useColoredLabel();
        App_View_Table::initDisplay(display);
    }
};

#endif
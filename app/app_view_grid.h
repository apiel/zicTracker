#ifndef APP_VIEW_GRID_H_
#define APP_VIEW_GRID_H_

#include "./app_tracks.h"
#include <app_core_renderer.h>
#include <app_core_util.h>
#include <app_core_view_table.h>

#ifndef VIEW_GRID_ROW
#define VIEW_GRID_ROW 8
#endif

#define VIEW_GRID_COL TRACK_COUNT * 3

class App_View_GridField : public App_View_TableField {
public:
    App_Tracks* tracks;
    char* description;

    App_View_GridField(App_Tracks* _tracks, char* _description)
        : tracks(_tracks)
        , description(_description)
    {
    }

    virtual bool isSelectable(uint8_t row, uint8_t col) override
    {
        return true;
    }

    virtual void selectCol0(App_Renderer* renderer, uint8_t row, uint8_t col) = 0;
    virtual void selectCol1(App_Renderer* renderer, uint8_t row, uint8_t col) { }
    virtual void selectCol2(App_Renderer* renderer, uint8_t row, uint8_t col) { }

    virtual void selected(App_Renderer* renderer, uint8_t row, uint8_t col) override
    {
        renderer->setCursor(2, col % 3 == 0 ? 1 : 0);
        if (col % 3 == 0) {
            selectCol0(renderer, row, col);
        } else if (col % 3 == 1) {
            selectCol1(renderer, row, col);
        } else if (col % 3 == 2) {
            selectCol2(renderer, row, col);
        }
    }

    virtual void renderCol0(App_Renderer* renderer, uint8_t row, uint8_t col) = 0;
    virtual void renderCol1(App_Renderer* renderer, uint8_t row, uint8_t col) { }
    virtual void renderCol2(App_Renderer* renderer, uint8_t row, uint8_t col) { }

    void render(App_Renderer* renderer, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        if (col % 3 == 0) {
            renderCol0(renderer, row, col);
        } else if (col % 3 == 1) {
            renderCol1(renderer, row, col);
        } else if (col % 3 == 2) {
            renderCol2(renderer, row, col);
        }
    }

    virtual uint8_t updateCol0(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col) = 0;
    virtual uint8_t updateCol1(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col) { return VIEW_NONE; }
    virtual uint8_t updateCol2(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col) { return VIEW_NONE; }

    uint8_t update(UiKeys* keys, App_Renderer* renderer, uint8_t row, uint8_t col)
    {
        if (col % 3 == 0) {
            return updateCol0(keys, renderer, row, col);
        } else if (col % 3 == 1) {
            return updateCol1(keys, renderer, row, col);
        } else if (col % 3 == 2) {
            return updateCol2(keys, renderer, row, col);
        }
        return VIEW_NONE;
    }
};

class App_View_Grid : public App_View_Table {
protected:
    App_View_TableField* field;

    App_View_TableField* fields[VIEW_GRID_ROW * VIEW_GRID_COL] = {
        // clang-format off
        field, field, field, field, field, field, field, field, field, field, field, field,
        field, field, field, field, field, field, field, field, field, field, field, field,
        field, field, field, field, field, field, field, field, field, field, field, field,
        field, field, field, field, field, field, field, field, field, field, field, field,
        field, field, field, field, field, field, field, field, field, field, field, field,
        field, field, field, field, field, field, field, field, field, field, field, field,
        field, field, field, field, field, field, field, field, field, field, field, field,
        field, field, field, field, field, field, field, field, field, field, field, field,
        // clang-format on
    };

public:
    char description[30] = "";

    App_View_Grid(App_View_TableField* _field)
        : App_View_Table(fields, VIEW_GRID_ROW, VIEW_GRID_COL)
        , field(_field)
    {
    }

    bool renderOn(uint8_t event) override
    {
        return true;
    }

    void initDisplay(App_Renderer* renderer)
    {
    }

    void render(App_Renderer* renderer)
    {
        renderer->useColoredRow();
        renderer->useColoredRow(9, COLOR_MEDIUM);
        strcpy(renderer->text, "");
        for (uint8_t i = 0; i < TRACK_COUNT; i++) {
            sprintf(renderer->text + strlen(renderer->text), " TRACK%d", i + 1);
        }
        strcat(renderer->text, "\n");
        App_View_Table::render(renderer);
        sprintf(renderer->text + strlen(renderer->text), " %s", description);
    }
};

#endif
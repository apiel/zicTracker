#ifndef APP_VIEW_GRID_H_
#define APP_VIEW_GRID_H_

#include "./app_tracks.h"
#include <app_core_renderer.h>
#include <app_core_util.h>
#include <app_core_view_table.h>

#define VIEW_GRID_ROW APP_TRACK_STATE_SIZE

#define GRID_VISIBLE_TRACKS 4
#define VIEW_GRID_COL GRID_VISIBLE_TRACKS * 3

uint8_t baseTrack = 0;

class App_View_GridField : public App_View_TableField {
public:
    App_Tracks* tracks;
    char* description;

    App_View_GridField(char* _description)
        : description(_description)
    {
        tracks = App_Tracks::getInstance();
    }

    App_Audio_Track* getTrack(uint8_t col)
    {
        return tracks->tracks[baseTrack + (uint8_t(col / 3) % GRID_VISIBLE_TRACKS)];
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

    virtual void renderCol0(App_Renderer* renderer, uint8_t row, uint8_t col, bool isSelected) = 0;
    virtual void renderCol1(App_Renderer* renderer, uint8_t row, uint8_t col, bool isSelected) { }
    virtual void renderCol2(App_Renderer* renderer, uint8_t row, uint8_t col, bool isSelected) { }

    void render(App_Renderer* renderer, uint8_t row, uint8_t col, uint8_t selectedRow, uint8_t selectedCol)
    {
        bool isSelected = selectedRow == row && selectedCol == col;
        if (col % 3 == 0) {
            App_Audio_Track* track = tracks->tracks[uint8_t(col / 3) % GRID_VISIBLE_TRACKS];
            renderer->useColor(row + 1, col / 3 * 7, track->looper.isComponentPlaying(row) ? COLOR_PLAY : COLOR_HILIGHT);
            strcat(renderer->text,
                track->looper.isComponentPlaying(row) ? ">"
                                                      : (track->looper.isCurrentComponent(row) ? "*" : " "));

            renderCol0(renderer, row, col, isSelected);
        } else if (col % 3 == 1) {
            renderCol1(renderer, row, col, isSelected);
        } else if (col % 3 == 2) {
            renderCol2(renderer, row, col, isSelected);
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

    void onSelectRow() override
    {
        gridSelectedRow = selectedRow;
    }

    void onSelectCol() override
    {
        gridSelectedCol = selectedCol;
    }

public:
    char description[30] = "";
    static uint8_t gridSelectedCol;
    static uint8_t gridSelectedRow;

    App_View_Grid(App_View_TableField* _field)
        : App_View_Table(fields, VIEW_GRID_ROW, VIEW_GRID_COL)
        , field(_field)
    {
    }

    static uint8_t getTrackId()
    {
        return baseTrack + (uint8_t(gridSelectedCol / 3) % GRID_VISIBLE_TRACKS);
    }

    bool renderOn(uint8_t event) override
    {
        return true;
    }

    void preRender(App_Renderer* renderer)
    {
    }

    virtual void setGridSelection()
    {
        selectedCol = gridSelectedCol;
        selectedRow = gridSelectedRow;
    }

    void focusView() override
    {
        setGridSelection();
    }

    void render(App_Renderer* renderer)
    {
        renderer->useColoredRow();
        renderer->useColoredRow(9, COLOR_MEDIUM);
        strcpy(renderer->text, "");
        App_Tracks* tracks = App_Tracks::getInstance();
        for (uint8_t i = 0; i < GRID_VISIBLE_TRACKS; i++) {
            // sprintf(renderer->text + strlen(renderer->text), " TRACK%d", baseTrack + i + 1);
            sprintf(renderer->text + strlen(renderer->text), " %s", tracks->tracks[baseTrack + i]->name);
        }
        strcat(renderer->text, "\n");
        App_View_Table::render(renderer);
        sprintf(renderer->text + strlen(renderer->text), " %s", description);
    }

    void onNextColOverflow(int8_t direction) override
    {
        int8_t nextBaseTrack = baseTrack + direction * GRID_VISIBLE_TRACKS;
        if (nextBaseTrack >= 0 && nextBaseTrack < TRACK_COUNT) {
            baseTrack = nextBaseTrack;
            if (direction > 0) {
                selectedCol = 0;
            } else {
                selectedCol = VIEW_GRID_COL - 1;
            }
        }
    }
};

uint8_t App_View_Grid::gridSelectedCol = 0;
uint8_t App_View_Grid::gridSelectedRow = 0;

#endif
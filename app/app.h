#ifndef APP_H_
#define APP_H_

#define APP_MENU_SIZE 10

#include "./app_def.h"
#include "./app_tracks.h"
#include "./app_view_instrument.h"
#include "./app_view_pattern.h"
#include "./app_view_project.h"
#include "./app_view_projectEditName.h"
#include "./app_view_comingSoon.h"
// #include "./app_view_track.h"
// #include "./app_view_trackDelay.h"
#include "./app_view_menu.h"
#include "./app_view_gridPattern.h"
#include "./app_view_gridInstrument.h"
#include "./app_view_help.h"
#include <app_core_display.h>
#include <app_core_renderer.h>
#include "./app_tempo.h"

class App {

protected:
    App_Tempo * tempo;

    App(App_Display* _display)
        : menuView(&menu[0])
    {
        App::display = _display;
        tempo = App_Tempo::getInstance();
        menuView.initMenu();
        tracks = App_Tracks::getInstance();
    }

public:
    App_Tracks * tracks;

    App_Display* display;
    UiKeys keys;

    App_View_Menu menuView;


    bool rendered = false;

    Menu menu[APP_MENU_SIZE] = {
        // NOTE Group menu per decimal number
        (Menu) { 10, "Grid: Pattern sequencer", "Pattern", App_View_GridPattern::getInstance() },
        (Menu) { 11, "Grid: Instrument seq.", "Instr.", App_View_GridInstrument::getInstance() },
        // instrument macro?
        // (Menu) { 12, "Grid: Effect sequencer", "IFX", NULL }, // Do we want custom effect per track OR 2 global effects and effects per patch? 
        (Menu) { 13, "Grid: Volume & Master FX", "VOL+MFX", App_View_ComingSoon::getInstance() },
        (Menu) { 20, "Edit: Pattern", "Pattern", App_View_Pattern::getInstance() },
        (Menu) { 21, "Edit: Instrument", "Instr.", App_View_Instrument::getInstance() },
        // (Menu) { 22, "Edit: Effect", "IFX", NULL },
        (Menu) { 30, "Scatter effect", "Scatter", App_View_ComingSoon::getInstance() },
        (Menu) { 31, "Master filter & effect", "MF+MFX", App_View_ComingSoon::getInstance() },
        (Menu) { 40, "Project", "Project", App_View_Project::getInstance(&menuView), true },
        (Menu) { 41, "Edit project name", "Name", App_View_ProjectEditName::getInstance(&menuView) },
        (Menu) { 50, "Help", "Help", App_View_Help::getInstance() },
    };

    static App* instance;

    static App* getInstance(App_Display* _display)
    {
        if (!instance) {
            instance = new App(_display);
        }
        return instance;
    }

    void sample(float* buf, int len)
    {
        if (tempo->next(SDL_GetTicks64()))
        // if (tempo.next())
        {
            tracks->next();
            if (menuView.getView()->renderOn(EVENT_VIEW_ON_TEMPO)) {
                render();
            }
        }
        tracks->sample(buf, len);
    }

    void render()
    {
        if (App::display->ready()) {
            // TODO find a better place way to reset cursor
            App::display->reset();
            menuView.getView()->render(App::display);
            App::display->drawText();
            rendered = true;
        }
    }

    void handleUi(uint8_t keysBin)
    {
        keys.Up = (keysBin >> UI_KEY_UP) & 1;
        keys.Down = (keysBin >> UI_KEY_DOWN) & 1;
        keys.Left = (keysBin >> UI_KEY_LEFT) & 1;
        keys.Right = (keysBin >> UI_KEY_RIGHT) & 1;
        keys.Edit = (keysBin >> UI_KEY_EDIT) & 1;
        keys.Menu = (keysBin >> UI_KEY_MENU) & 1;
        // SDL_Log("%d%d%d%d%d%d\n", keys.Up, keys.Down, keys.Left, keys.Right, keys.A, keys.Y);

        if (keys.Menu && keys.Edit) {
            tracks->togglePlay();
        } else if (menuView.update(&keys, App::display) != VIEW_NONE) {
            render();
        } else if (menuView.getView()->update(&keys, App::display) != VIEW_NONE) {
            render();
        }
    }

    void quit()
    {
        App_Renderer renderer;
        for (uint8_t i; i < APP_MENU_SIZE; i++) {
            if (menu[i].view) {
                renderer.reset();
                renderer.startRow = 0;
                // TODO need to use different way to save project
                // menu[i].view->snapshot(&renderer);
            }
        }

        tracks->quit();
        App_State::getInstance()->save();
    }

    void start()
    {
        App_State::getInstance()->load();

        for (uint8_t i; i < APP_MENU_SIZE; i++) {
            if (menu[i].view) {
                menu[i].view->loadSnapshot();
            }
        }
    }
};

App* App::instance = NULL;

#endif
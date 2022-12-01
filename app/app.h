#ifndef APP_H_
#define APP_H_

#include "./app_def.h"
#include "./app_project.h"
#include "./app_tracks.h"
#include "./app_view_instrument.h"
#include "./app_view_menu.h"
#include "./app_view_pattern.h"
#include "./app_view_project.h"
#include "./app_view_projectEditName.h"
// #include "./app_view_track.h"
// #include "./app_view_trackDelay.h"
#include "./app_view_trackSequencer.h"
#include <app_core_display.h>
#include <app_core_renderer.h>

#include <zic_seq_tempo.h>

#define APP_MENU_SIZE 9

class App {

protected:
    App(App_Display* _display)
        : menuView(&menu[0], APP_MENU_SIZE)
    {
        App::display = _display;
    }

public:
    App_Tracks tracks;
    Zic_Seq_Tempo<> tempo;
    App_Project project;

    Zic_Seq_Pattern patterns[PATTERN_COUNT];

    static App_Display* display;
    UiKeys keys;

    App_View_Menu menuView;

    bool rendered = false;

    Menu menu[APP_MENU_SIZE] = {
        (Menu) { "Tracks sequencer", "Sequencer", 'S', App_View_TrackSequencer::getInstance(&tracks, &patterns[0]), 'T', true },
        (Menu) { "Tracks", "Tracks", 'T', NULL, 'T', false },
        (Menu) { "Track delay", "Delay", 'D', NULL, 'T', false },
        (Menu) { "Instruments", "Instruments", 'I', App_View_Instrument::getInstance(&tracks), 'I', true },
        (Menu) { "Instruments kit", "Kit", 'K', NULL, 'I', false }, // this is how to save a kit // should it be called presets?
        (Menu) { "Pattern", "Pattern", 'P', App_View_Pattern::getInstance(&patterns[0]), 'P', true },
        (Menu) { "Sampler", "Sampler", 'S', NULL, 'S', true }, // Record all track to sample and edit sample
        (Menu) { "Project", "Project", 'J', App_View_Project::getInstance(&tempo, &tracks, &project, &menuView), 'J', true }, // Select project
        (Menu) { "Edit project name", "Name", 'N', App_View_ProjectEditName::getInstance(&project, &menuView), 'J', false }, // Select project
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
        if (tempo.next(SDL_GetTicks64()))
        // if (tempo.next())
        {
            tracks.next();
            if (menuView.getView()->renderOn(EVENT_VIEW_ON_TEMPO)) {
                render();
            }
        }
        tracks.sample(buf, len);
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
            tracks.togglePlay();
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
    }

    void start()
    {
        for (uint8_t i; i < APP_MENU_SIZE; i++) {
            if (menu[i].view) {
                menu[i].view->loadSnapshot();
            }
        }
    }
};

App_Display* App::display = NULL;
App* App::instance = NULL;

#endif
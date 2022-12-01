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

// TODO make app singleton
class App {
public:
    App_Tracks tracks;
    Zic_Seq_Tempo<> tempo;
    App_Project project;

    Zic_Seq_Pattern patterns[PATTERN_COUNT];

    static App_Display* display;
    UiKeys keys;

    // App_View_Track trackView;
    App_View_TrackSequencer trackSeqView;
    // App_View_Instrument instrumentView;
    App_View_Pattern patternView;
    // App_View_TrackDelay trackDelayView;
    App_View_Project projectView;
    App_View_ProjectEditName projectEditNameView;

    App_View_Menu menuView;

    bool rendered = false;

#define APP_MENU_SIZE 9
    Menu menu[APP_MENU_SIZE] = {
        (Menu) { "Tracks sequencer", "Sequencer", 'S', &trackSeqView, 'T', true },
        // (Menu) { "Tracks", "Tracks", 'T', &trackView, 'T', false },
        // (Menu) { "Track delay", "Delay", 'D', &trackDelayView, 'T', false },
        (Menu) { "Tracks", "Tracks", 'T', NULL, 'T', false },
        (Menu) { "Track delay", "Delay", 'D', NULL, 'T', false },
        // (Menu) { "Instruments", "Instruments", 'I', &instrumentView, 'I', true },
        (Menu) { "Instruments", "Instruments", 'I', App_View_Instrument::getInstance(&tracks), 'I', true },
        (Menu) { "Instruments kit", "Kit", 'K', NULL, 'I', false }, // this is how to save a kit // should it be called presets?
        // 4 LFO -> can be assigned to any changeable values and can be use for multiple instrument at the same time
        // 4 Extra Envelop -> same as LFO
        // IFX: reverb, distortion...
        // { "Project", 'P', VIEW_TRACK_PROJECT, 'T', false }, // Select project
        (Menu) { "Pattern", "Pattern", 'P', &patternView, 'P', true },
        (Menu) { "Sampler", "Sampler", 'S', NULL, 'S', true }, // Record all track to sample and edit sample
        (Menu) { "Project", "Project", 'J', &projectView, 'J', true }, // Select project
        (Menu) { "Edit project name", "Name", 'N', &projectEditNameView, 'J', false }, // Select project
    };

    App(App_Display* _display)
        : trackSeqView(&tracks, &patterns[0])
        // , instrumentView(&tracks)
        , patternView(&patterns[0])
        // , trackDelayView(&tracks)
        , projectView(&tempo, &tracks, &project, &menuView)
        , projectEditNameView(&project, &menuView)
        , menuView(&menu[0], APP_MENU_SIZE)
    {
        App::display = _display;
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

#endif
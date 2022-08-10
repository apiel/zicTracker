#ifndef APP_H_
#define APP_H_

#include "./app_def.h"
#include "./app_display.h"
#include "./app_patterns.h"
#include "./app_tracks.h"
#include "./app_view_instrument.h"
#include "./app_view_menu.h"
#include "./app_view_pattern.h"
#include "./app_view_project.h"
#include "./app_view_track.h"
#include "./app_view_trackDelay.h"
#include "./app_view_trackSequencer.h"
#include <zic_seq_tempo.h>

class App {
public:
    Zic_Seq_Tempo<> tempo;
    App_Tracks tracks;

    App_Display* display;
    UiKeys keys;

    App_View_Menu menuView;
    App_View_Track trackView;
    App_View_TrackSequencer trackSeqView;
    App_View_Instrument instrumentView;
    App_View_Pattern patternView;
    App_View_TrackDelay trackDelayView;
    App_View_Project projectView;

    App(App_Patterns* patterns, App_Display* _display)
        : tracks(patterns)
        , display(_display)
        , trackView(&tracks)
        , trackSeqView(&tracks)
        , instrumentView(&tracks)
        , patternView(patterns)
        , trackDelayView(&tracks)
        , projectView(&tempo)
    {
        menuView.add(&menuView)
            ->add(&trackView)
            ->add(&trackSeqView)
            ->add(&instrumentView)
            ->add(&patternView)
            ->add(&trackDelayView)
            ->add(&projectView);
    }

    void start()
    {
        // tracks.looper->setLoopMode(true);
        // tracks.looper->on(60);
    }

    int16_t sample()
    {
        if (tempo.next()) {
            tracks.next();
        }
        return tracks.sample();
    }

    void render()
    {
        // TODO find a better place way to reset cursor
        display->reset();
        menuView.getView()->render(display);

        // App_View * view = menuView.getView();
        // switch (view) {
        // case VIEW_MENU:
        //     menuView.render(display);
        //     break;
        // case VIEW_TRACK_SEQUENCER:
        //     trackSeqView.render(display);
        //     break;

        // case VIEW_TRACK_DELAY:
        //     trackDelayView.render(display);
        //     break;

        // case VIEW_INSTRUMENT:
        //     instrumentView.render(display);
        //     break;

        // case VIEW_PATTERN:
        //     patternView.render(display);
        //     break;

        // case VIEW_PROJECT:
        //     projectView.render(display);
        //     break;

        // default:
        //     trackView.render(display);
        //     break;
        // }

        display->drawText();
    }

    void handleUi(uint8_t keysBin)
    {
        keys.Up = (keysBin >> UI_KEY_UP) & 1;
        keys.Down = (keysBin >> UI_KEY_DOWN) & 1;
        keys.Left = (keysBin >> UI_KEY_LEFT) & 1;
        keys.Right = (keysBin >> UI_KEY_RIGHT) & 1;
        keys.A = (keysBin >> UI_KEY_A) & 1;
        keys.B = (keysBin >> UI_KEY_B) & 1;
        // SDL_Log("%d%d%d%d%d%d\n", keys.Up, keys.Down, keys.Left, keys.Right, keys.A, keys.Y);

        if (menuView.update(&keys, display) != VIEW_NONE) {
            render();
        } else {
            // uint8_t viewUpdated = VIEW_NONE;
            // uint8_t view = menuView.getView();
            // switch (view) {
            // case VIEW_TRACK:
            //     viewUpdated = trackView.update(&keys, display);
            //     break;
            // case VIEW_TRACK_DELAY:
            //     viewUpdated = trackDelayView.update(&keys, display);
            //     break;
            // case VIEW_TRACK_SEQUENCER:
            //     viewUpdated = trackSeqView.update(&keys, display);
            //     break;
            // case VIEW_INSTRUMENT:
            //     viewUpdated = instrumentView.update(&keys, display);
            //     break;
            // case VIEW_PATTERN:
            //     viewUpdated = patternView.update(&keys, display);
            //     break;
            // case VIEW_PROJECT:
            //     viewUpdated = projectView.update(&keys, display);
            //     break;
            // }
            uint8_t viewUpdated = menuView.getView()->update(&keys, display);
            if (viewUpdated != VIEW_NONE) {
                render();
            }
        }
    }
};

#endif
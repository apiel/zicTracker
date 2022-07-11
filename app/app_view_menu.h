#ifndef APP_VIEW_MENU_H_
#define APP_VIEW_MENU_H_

#include "./app_display.h"
#include "./app_view.h"

#define APP_MENU_SIZE 7

typedef struct {
    const char* name;
    char key;
    uint8_t view;
    char group;
    bool selected;
} Menu;

class App_View_Menu : App_View {
protected:
    void inc(int8_t val)
    {
        // or could be:
        // currentMenu = ((currentMenu ? currentMenu : APP_MENU_SIZE) + val) % APP_MENU_SIZE;
        currentMenu = (currentMenu + val);
        if (currentMenu == 255) {
            currentMenu = APP_MENU_SIZE - 1;
        }
        currentMenu = currentMenu % APP_MENU_SIZE;
    }

    void menuInc(int8_t val)
    {
        menu[currentMenu].selected = false;
        char group = menu[currentMenu].group;
        inc(val);
        if (group != menu[currentMenu].group) {
            inc(-val);
        }
        menu[currentMenu].selected = true;
    }

    void groupInc(int8_t val)
    {
        do {
            inc(val);
        } while (!menu[currentMenu].selected);
    }

    void menuPlus()
    {
        menuInc(+1);
    }

    void menuMinus()
    {
        menuInc(-1);
    }

    void groupPlus()
    {
        groupInc(+1);
    }

    void groupMinus()
    {
        groupInc(-1);
    }

public:
    UiKeys keys;

    Menu menu[APP_MENU_SIZE] = {
        { "Project", 'P', VIEW_TRACK, 'P', true },
        { "Tracks", 'T', VIEW_TRACK, 'T', true },
        { "Track Loop", 'L', VIEW_TRACK_LOOP, 'T', false },
        { "Instruments", 'I', VIEW_INSTRUMENT, 'I', true },
        { "Instruments kit", 'K', VIEW_INSTRUMENT, 'I', false },
        // { "Project", 'P', VIEW_TRACK_PROJECT, 'T', false }, // Select project
        { "Pattern", 'P', VIEW_PATTERN, 'P', true },
        { "Wavetable sampler", 'W', VIEW_WAVETABLE_SAMPLER, 'S', true }, // generate some wavetable by combining multiple wavetable and detune, effect?, lfo?
    };
    uint8_t currentMenu = 0;

    uint8_t getView()
    {
        return menu[currentMenu].view;
    }

    void render(App_Display* display)
    {
        strcpy(display->text, "");
        for (uint8_t i = 0; i < APP_MENU_SIZE; i++) {
            if (i != 0 && menu[i].group != menu[i - 1].group) {
                strcat(display->text, "\n");
            }
            if (i == currentMenu) {
                display->setCursor(1, 1);
            }
            sprintf(display->text + strlen(display->text), " %c", menu[i].key);
        }
        sprintf(display->text + strlen(display->text), "\n\n %s", menu[currentMenu].name);
    }

    uint8_t update(UiKeys* keys, App_Display* display)
    {
        if (keys->B) {
            if (keys->Right) {
                menuPlus();
            } else if (keys->Left) {
                menuMinus();
            } else if (keys->Up) {
                groupMinus();
            } else if (keys->Down) {
                groupPlus();
            }
            render(display);
            return VIEW_CHANGED;
        }
        return VIEW_NONE;
    }
};

#endif
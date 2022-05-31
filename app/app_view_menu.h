#ifndef APP_VIEW_MENU_H_
#define APP_VIEW_MENU_H_

#include "./app_def.h"

#include <cstring>
#include <stdio.h>

#define APP_MENU_SIZE 7

typedef struct {
    const char* name;
    const char key;
    uint8_t view;
    bool isBase = false;
} Menu;

class App_View_Menu {
protected:
    void menuPlus()
    {
        currentMenu = (currentMenu + 1) % APP_MENU_SIZE;
    }

    void menuMinus()
    {
        currentMenu--;
        if (currentMenu == 255) {
            currentMenu = APP_MENU_SIZE - 1;
        }
    }

public:
    UiKeys keys;

    Menu menu[APP_MENU_SIZE] = {
        { "Track", 'T', VIEW_TRACK, true },
        { "Track Loop", 'L', VIEW_TRACK_LOOP },
        { "Track pattern", 'P', VIEW_TRACK_PATTERN },
        { "Pattern", 'P', VIEW_PATTERN, true },
        { "Pattern edit", 'E', VIEW_PATTERN_EDIT },
        { "Instrument", 'I', VIEW_INSTRUMENT, true },
        { "Instrument edit", 'E', VIEW_INSTRUMENT_EDIT },
    };
    uint8_t currentMenu = 0;

    uint8_t render(UiKeys* keys, char& display)
    {
        if (keys->A) {
            if (keys->Right) {
                menuPlus();
            } else if (keys->Left) {
                menuMinus();
            } else if (keys->Up) {
                do {
                    menuMinus();
                } while (!menu[currentMenu].isBase);
            } else if (keys->Down) {
                do {
                    menuPlus();
                } while (!menu[currentMenu].isBase);
            }
            strcpy(&display, "");
            for (uint8_t i = 0; i < APP_MENU_SIZE; i++) {
                if (i != 0 && menu[i].isBase) {
                    strcat(&display, "\n");
                }
                if (i == currentMenu) {
                    sprintf(&display + strlen(&display), "~1%c~0 ", menu[i].key);
                } else {
                    sprintf(&display + strlen(&display), "%c ", menu[i].key);
                }
            }
            sprintf(&display + strlen(&display), "\n\n%s", menu[currentMenu].name);
            return VIEW_DONE;
        }
        return VIEW_NONE;
    }
};

#endif
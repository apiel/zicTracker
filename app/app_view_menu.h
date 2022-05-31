#ifndef APP_VIEW_MENU_H_
#define APP_VIEW_MENU_H_

#include "./app_def.h"

#include <cstring>
#include <stdio.h>

#define APP_MENU_SIZE 7

typedef struct {
    const char* name;
    const char key;
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
        { "Track", 'T', true },
        { "Track Loop", 'L' },
        { "Track pattern", 'P' },
        { "Pattern", 'P', true },
        { "Pattern edit", 'E' },
        { "Instrument", 'I', true },
        { "Instrument edit", 'E' },
    };
    uint8_t currentMenu = 0;

    uint8_t render(UiKeys* keys, char &display)
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
#ifndef APP_VIEW_H_
#define APP_VIEW_H_

#include "./app_def.h"
#include "./app_display.h"

#include <cstring>
#include <stdio.h>

class App_View {
public:
    uint8_t viewId = 0;

    virtual void render(App_Display* display) = 0;
    virtual uint8_t update(UiKeys* keys, App_Display* display) = 0;

    App_View(u_int8_t _viewId)
        : viewId(_viewId)
    {
    }
};

#endif

#ifndef APP_VIEW_LOOPER_H_
#define APP_VIEW_LOOPER_H_

#include "./app_def.h"

class App_View_Looper {
public:
    uint8_t render(UiKeys* keys, char& display)
    {
        sprintf(&display, "Loooooooper");
        return VIEW_NONE;
    }
};

#endif
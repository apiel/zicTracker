#ifndef APP_TEMPO_H_
#define APP_TEMPO_H_

#include <zic_seq_tempo.h>

class App_Tempo: public Zic_Seq_Tempo<> {
    protected:
        App_Tempo() {}

public:
    static App_Tempo* instance;

    static App_Tempo* getInstance()
    {
        if (!instance) {
            instance = new App_Tempo();
        }
        return instance;
    }
};

App_Tempo* App_Tempo::instance = NULL;

#endif
#ifndef APP_VIEW_TRACK_LOOP_H_
#define APP_VIEW_TRACK_LOOP_H_

#include "./app_tracks.h"
#include "./app_view.h"

#ifndef APP_VIEW_TRACK_LOOP_COL
#define APP_VIEW_TRACK_LOOP_COL 4
#endif

#ifndef APP_VIEW_TRACK_LOOP_ROW
#define APP_VIEW_TRACK_LOOP_ROW 5
#endif

class App_View_TrackLoop : App_View {
protected:
    App_Tracks* tracks;

    uint8_t pos = 3 * 7;
    uint8_t sel = 4 * 7;
    uint8_t play = 4 * 7;

public:
    App_View_TrackLoop(App_Tracks* _tracks)
        : tracks(_tracks)
    {
    }

    void render(char* display)
    {
        char octave[1];
        strcpy(display, "");
        for (uint8_t row = 0, n = 0; row < APP_VIEW_TRACK_LOOP_ROW; row++, n += 8 - APP_VIEW_TRACK_LOOP_ROW) {
            for (uint8_t col = 0; col < APP_VIEW_TRACK_LOOP_COL; col++, n++) {
                if (col != 0) {
                    strcat(display, " ");
                }
                uint8_t note = naturalNotes[n + pos];
                bool colored = false;
                if (sel == n + pos) {
                    strcat(display, "~b");
                    colored = true;
                }
                if (play == n + pos) {
                    strcat(display, "~1");
                    colored = true;
                }
                strcat(display, getNoteStr(note));
                octave[0] = '0' + getNoteOctave(note);
                strcat(display, octave);
                if (colored) {
                    strcat(display, "~0");
                }
            }
            strcat(display, "\n");
        }
    }

    uint8_t update(UiKeys* keys, char* display)
    {
        if (keys->Up || keys->Down || keys->Right || keys->Left) {
            // if (keys->Up) {
            //     if ((int16_t)pos - 7 >= 0) {
            //         pos -= 7;
            //     }
            // } else if (keys->Down) {
            //     if (pos + 7 < (10 - APP_VIEW_TRACK_LOOP_ROW) * 7) {
            //         pos += 7;
            //     }
            // } else if (keys->Right) {
            //     if (pos % 7 != 8 - APP_VIEW_TRACK_LOOP_ROW) {
            //         pos++;
            //     }
            // } else if (keys->Left) {
            //     if (pos % 7 != 0) {
            //         pos--;
            //     }
            // }
            if (keys->Up) {
                if ((int16_t)sel - 7 >= 0) {
                    sel -= 7;
                    if (sel < pos) {
                        pos -= 7;
                    }
                }
            } else if (keys->Down) {
                if (sel + 7 < 8 * 7) {
                    sel += 7;
                    // if (sel > pos) {
                    //     pos += 7;
                    // }
                }
            } else if (keys->Right) {
                if (sel % 7 < 6) {
                    sel++;
                    if (sel % 7 > 7 - APP_VIEW_TRACK_LOOP_COL) {
                        pos++;
                    }
                }
            } else if (keys->Left) {
                if (sel % 7 != 0) {
                    sel--;
                    if (sel % 7 < pos % 7) {
                        pos--;
                    }
                }
            }
            SDL_Log("pos %d,%d sel %d,%d", pos, pos % 7, sel, sel % 7);
            render(display);
            return VIEW_CHANGED;
        }
        return VIEW_NONE;
    }
};

#endif
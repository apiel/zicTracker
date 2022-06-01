#ifndef APP_VIEW_TRACK_LOOP_H_
#define APP_VIEW_TRACK_LOOP_H_

#include "./app_tracks.h"
#include "./app_view_table.h"

#ifndef APP_VIEW_TRACK_LOOP_COL
#define APP_VIEW_TRACK_LOOP_COL 4
#endif

#ifndef APP_VIEW_TRACK_LOOP_ROW
#define APP_VIEW_TRACK_LOOP_ROW 5
#endif

class App_View_TrackLoop : public App_View_Table<5, 4, 7> {
protected:
    App_Tracks* tracks;

    // uint8_t pos = 3 * 7;
    // uint8_t cursor = 4 * 7;
    uint8_t playNote = 60;

public:
    App_View_TrackLoop(App_Tracks* _tracks)
        : tracks(_tracks)
    {
    }

    void renderPos(char* display, uint16_t _pos)
    {
        char octave[1];
        uint8_t note = naturalNotes[_pos];
        bool colored = false;
        if (cursor == _pos) {
            strcat(display, "~b");
            colored = true;
        }
        if (playNote == note) {
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

    // void render(char* display)
    // {
    //     char octave[1];
    //     strcpy(display, "");
    //     for (uint8_t row = 0, n = 0; row < APP_VIEW_TRACK_LOOP_ROW; row++, n += 8 - APP_VIEW_TRACK_LOOP_ROW) {
    //         for (uint8_t col = 0; col < APP_VIEW_TRACK_LOOP_COL; col++, n++) {
    //             if (col != 0) {
    //                 strcat(display, " ");
    //             }
    //             uint8_t note = naturalNotes[n + pos];
    //             bool colored = false;
    //             if (cursor == n + pos) {
    //                 strcat(display, "~b");
    //                 colored = true;
    //             }
    //             if (playNote == note) {
    //                 strcat(display, "~1");
    //                 colored = true;
    //             }
    //             strcat(display, getNoteStr(note));
    //             octave[0] = '0' + getNoteOctave(note);
    //             strcat(display, octave);
    //             if (colored) {
    //                 strcat(display, "~0");
    //             }
    //         }
    //         strcat(display, "\n");
    //     }
    // }

    uint8_t update(UiKeys* keys, char* display)
    {
        if (keys->Up || keys->Down || keys->Right || keys->Left) {
            if (keys->Up) {
                if ((int16_t)cursor - 7 >= 0) {
                    cursor -= 7;
                    if (cursor < pos) {
                        pos -= 7;
                    }
                }
            } else if (keys->Down) {
                if (cursor + 7 < 8 * 7) {
                    cursor += 7;
                    // FIXME fixme
                    if (cursor > pos + (7 * APP_VIEW_TRACK_LOOP_ROW)) {
                        pos += 7;
                    }
                }
            } else if (keys->Right) {
                if (cursor % 7 < 6) {
                    cursor++;
                    if (cursor % 7 > 7 - APP_VIEW_TRACK_LOOP_COL) {
                        pos++;
                    }
                }
            } else if (keys->Left) {
                if (cursor % 7 != 0) {
                    cursor--;
                    if (cursor % 7 < pos % 7) {
                        pos--;
                    }
                }
            }
            SDL_Log("pos %d,%d cursor %d,%d", pos, pos % 7, cursor, cursor % 7);
            render(display);
            return VIEW_CHANGED;
        }
        return VIEW_NONE;
    }
};

#endif
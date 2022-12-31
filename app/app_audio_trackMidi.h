#ifndef APP_AUDIO_TRACK_MIDI_H_
#define APP_AUDIO_TRACK_MIDI_H_

#include "./app_audio_track.h"

#include <RtMidi.h>
#include <zic_note.h>

class App_Audio_TrackMidi : public App_Audio_Track {
public:
    RtMidiOut* midiout = 0;
    uint8_t port = 0;

    App_Audio_TrackMidi(uint8_t _id = 0, const char* _name = NULL)
        : App_Audio_Track(_id, _name)
    {
    }

    ~App_Audio_TrackMidi()
    {
        delete midiout;
    }

    bool isAudioTrack() override
    {
        return false;
    }

    void noteOn(uint8_t note, uint8_t velocity, uint8_t voice)
    {
        APP_LOG("MIDI note on %d %d %d\n", note, velocity, voice);
    }

    void noteOff(uint8_t note, uint8_t voice)
    {
        APP_LOG("MIDI note off %d %d\n", note, voice);
    }

    void sample(float* buf, int len)
    {
    }

    void cc(uint8_t num, uint8_t val, uint8_t voice) override
    {
        APP_LOG("MIDI cc %d %d %d\n", num, val, voice);
    }

    const char* getPatchDirectory() override
    {
        return "instruments/midi";
    }

    void setMidiOut(RtMidiOut* _midiout)
    {
        delete midiout;
        midiout = _midiout;
        setMidiOutPort(0);
    }

    void setMidiOutPort(int8_t direction)
    {
        int portCount = midiout->getPortCount() - 1;
        port = (port + direction + portCount) % portCount;
        midiout->openPort(port);
    }
};

#endif

#include <unistd.h>

#include <cstdlib>
#include <iostream>

#include "../zic/wavetables/wavetable_Bank.h"
#include "../zic/zic_wave_wavetable.h"
#include "RtAudio.h"

#define FORMAT RTAUDIO_SINT16
#define CHANNELS 1

#ifndef SAMPLE_RATE
#define SAMPLE_RATE 44100
#endif

void errorCallback(RtAudioErrorType /*type*/, const std::string& errorText)
{
    // This example error handling function simply outputs the error message to
    // stderr.
    std::cerr << "\nerrorCallback: " << errorText << "\n\n";
}

RtAudio::StreamOptions options;

Zic_Wave_Wavetable wave(&wavetable_Bank);

// Interleaved buffers
int saw(void* outputBuffer,
    void* /*inputBuffer*/,
    unsigned int nBufferFrames,
    double streamTime,
    RtAudioStreamStatus status,
    void* data)
{
    unsigned int i;
    int16_t* buffer = (int16_t*)outputBuffer;

    if (status) {
        std::cout << "Stream underflow detected!" << std::endl;
    }

    for (i = 0; i < nBufferFrames; i++) {
        *buffer++ = wave.next();
#if CHANNELS == 2
        *buffer++ = *(buffer - 1);
#endif
    }
    return 0;
}

int main(int argc, char* argv[])
{
    // Specify our own error callback function.
    RtAudio dac(RtAudio::UNSPECIFIED, &errorCallback);
    dac.showWarnings(true);

    double* data = (double*)calloc(CHANNELS, sizeof(double));

    // Set our stream parameters for output only.
    RtAudio::StreamParameters oParams;
    oParams.nChannels = CHANNELS;

    options.flags = RTAUDIO_HOG_DEVICE;
    options.flags |= RTAUDIO_SCHEDULE_REALTIME;

    unsigned int bufferFrames = 512;
    // An error in the openStream() function can be detected either by
    // checking for a non-zero return value OR by a subsequent call to
    // isStreamOpen().
    if (dac.openStream(&oParams,
            NULL,
            FORMAT,
            SAMPLE_RATE,
            &bufferFrames,
            &saw,
            (void*)data,
            &options)
            == 0
        && dac.isStreamOpen() == true && dac.startStream() == 0) {
            
        std::cout << "\nPlaying ... quit with Ctrl-C (buffer size = "
                  << bufferFrames << ").\n";

        while (dac.isStreamRunning()) {
            usleep(100000);
        }

        // Block released ... stop the stream
        if (dac.isStreamRunning()) {
            dac.stopStream(); // or could call dac.abortStream();
        }
    }

    if (dac.isStreamOpen()) {
        dac.closeStream();
    }
    free(data);

    return 0;
}


#include <iostream>
#include <cstdlib>
#include <unistd.h>

#include "RtAudio.h"

#include "../zic/zic_wave_wavetable.h"
#include "../zic/wavetables/wavetable_Bank.h"

#define FORMAT RTAUDIO_SINT16
#define CHANNELS 1

#ifndef SAMPLE_RATE
#define SAMPLE_RATE 44100
#endif

void errorCallback(RtAudioErrorType /*type*/, const std::string &errorText)
{
    // This example error handling function simply outputs the error message to stderr.
    std::cerr << "\nerrorCallback: " << errorText << "\n\n";
}

RtAudio::StreamOptions options;

Zic_Wave_Wavetable wave(&wavetable_Bank);

// Interleaved buffers
int saw(void *outputBuffer, void * /*inputBuffer*/, unsigned int nBufferFrames,
        double streamTime, RtAudioStreamStatus status, void *data)
{
    unsigned int i;
    int16_t *buffer = (int16_t *)outputBuffer;

    if (status)
    {
        std::cout << "Stream underflow detected!" << std::endl;
    }

    for (i = 0; i < nBufferFrames; i++)
    {
        *buffer++ = wave.next();
#if CHANNELS == 2
        *buffer++ = *(buffer - 1);
#endif
    }
    return 0;
}

int main(int argc, char *argv[])
{
    unsigned int bufferFrames, device = 0, offset = 0;

    // Specify our own error callback function.
    RtAudio dac(RtAudio::UNSPECIFIED, &errorCallback);

    if (dac.getDeviceCount() < 1)
    {
        std::cout << "\nNo audio devices found!\n";
        exit(1);
    }

    double *data = (double *)calloc(CHANNELS, sizeof(double));

    // Tell RtAudio to output all messages, even warnings.
    dac.showWarnings(true);

    // Set our stream parameters for output only.
    bufferFrames = 512;
    RtAudio::StreamParameters oParams;
    oParams.deviceId = device;
    oParams.nChannels = CHANNELS;
    oParams.firstChannel = offset;

    if (device == 0)
    {
        oParams.deviceId = dac.getDefaultOutputDevice();
    }

    options.flags = RTAUDIO_HOG_DEVICE;
    options.flags |= RTAUDIO_SCHEDULE_REALTIME;

    // An error in the openStream() function can be detected either by
    // checking for a non-zero return value OR by a subsequent call to
    // isStreamOpen().
    if (dac.openStream(&oParams, NULL, FORMAT, SAMPLE_RATE, &bufferFrames, &saw, (void *)data, &options))
    {
        goto cleanup;
    }
    if (dac.isStreamOpen() == false)
    {
        goto cleanup;
    }
    if (dac.startStream())
    {
        goto cleanup;
    }

    // if (checkCount)
    // {
    //     while (dac.isStreamRunning() == true)
    //     {
    //         usleep(100000);
    //     }
    // }
    // else
    // {
        std::cout << "\nPlaying ... quit with Ctrl-C (buffer size = " << bufferFrames << ").\n";

        while (dac.isStreamRunning())
        {
            usleep(100000);
        }

        // Block released ... stop the stream
        if (dac.isStreamRunning())
        {
            dac.stopStream(); // or could call dac.abortStream();
        }
    // }

cleanup:
    if (dac.isStreamOpen())
    {
        dac.closeStream();
    }
    free(data);

    return 0;
}

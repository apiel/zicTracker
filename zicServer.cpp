// g++ -Wall -D__LINUX_PULSE__ -o zicServer zicServer.cpp RtAudio.cpp -lpthread -lpulse-simple -lpulse

#include <iostream>
#include <cstdlib>
// #include <signal.h>

#include "RtAudio.h"

// #include "../zic/zic_wave_wavetable.h"

typedef signed short MY_TYPE;
#define FORMAT RTAUDIO_SINT16
#define SCALE 32767.0

/*
typedef float MY_TYPE;
#define FORMAT RTAUDIO_FLOAT32
#define SCALE  1.0
*/

#include <unistd.h>
#define SLEEP(milliseconds) usleep((unsigned long)(milliseconds * 1000.0))

// // Interrupt handler function
// bool done;
// static void finish(int /*ignore*/) { done = true; }

#define BASE_RATE 0.005
#define TIME 1.0

void errorCallback(RtAudioErrorType /*type*/, const std::string &errorText)
{
  // This example error handling function simply outputs the error message to stderr.
  std::cerr << "\nerrorCallback: " << errorText << "\n\n";
}

unsigned int channels;
RtAudio::StreamOptions options;
unsigned int frameCounter = 0;
bool checkCount = false;
unsigned int nFrames = 0;
const unsigned int callbackReturnValue = 1; // 1 = stop and drain, 2 = abort
double streamTimePrintIncrement = 1.0;      // seconds
double streamTimePrintTime = 1.0;           // seconds

// Interleaved buffers
int saw(void *outputBuffer, void * /*inputBuffer*/, unsigned int nBufferFrames,
        double streamTime, RtAudioStreamStatus status, void *data)
{
  unsigned int i, j;
  extern unsigned int channels;
  MY_TYPE *buffer = (MY_TYPE *)outputBuffer;
  double *lastValues = (double *)data;

  if (status)
    std::cout << "Stream underflow detected!" << std::endl;

  if (streamTime >= streamTimePrintTime)
  {
    std::cout << "streamTime = " << streamTime << std::endl;
    streamTimePrintTime += streamTimePrintIncrement;
  }

  for (i = 0; i < nBufferFrames; i++)
  {
    for (j = 0; j < channels; j++)
    {
      *buffer++ = (MY_TYPE)(lastValues[j] * SCALE * 0.5);
      lastValues[j] += BASE_RATE * (j + 1 + (j * 0.1));
      if (lastValues[j] >= 1.0)
        lastValues[j] -= 2.0;
    }
  }

  frameCounter += nBufferFrames;
  if (checkCount && (frameCounter >= nFrames))
    return callbackReturnValue;
  return 0;
}

int main(int argc, char *argv[])
{
  unsigned int bufferFrames, fs, device = 0, offset = 0;

  // Specify our own error callback function.
  RtAudio dac(RtAudio::UNSPECIFIED, &errorCallback);

  if (dac.getDeviceCount() < 1)
  {
    std::cout << "\nNo audio devices found!\n";
    exit(1);
  }

  channels = (unsigned int)1;
  fs = (unsigned int)44100;

  double *data = (double *)calloc(channels, sizeof(double));

  // Tell RtAudio to output all messages, even warnings.
  dac.showWarnings(true);

  // Set our stream parameters for output only.
  bufferFrames = 512;
  RtAudio::StreamParameters oParams;
  oParams.deviceId = device;
  oParams.nChannels = channels;
  oParams.firstChannel = offset;

  if (device == 0)
    oParams.deviceId = dac.getDefaultOutputDevice();

  options.flags = RTAUDIO_HOG_DEVICE;
  options.flags |= RTAUDIO_SCHEDULE_REALTIME;

  // An error in the openStream() function can be detected either by
  // checking for a non-zero return value OR by a subsequent call to
  // isStreamOpen().
  if (dac.openStream(&oParams, NULL, FORMAT, fs, &bufferFrames, &saw, (void *)data, &options))
    goto cleanup;
  if (dac.isStreamOpen() == false)
    goto cleanup;

  // std::cout << "Stream latency = " << dac.getStreamLatency() << "\n" << std::endl;

  if (dac.startStream())
  {
    goto cleanup;
  }

  if (checkCount)
  {
    while (dac.isStreamRunning() == true)
      SLEEP(100);
  }
  else
  {
    std::cout << "\nPlaying ... quit with Ctrl-C (buffer size = " << bufferFrames << ").\n";

    while (dac.isStreamRunning())
    {
      SLEEP(100);
    }

    // Block released ... stop the stream
    if (dac.isStreamRunning())
    {
      dac.stopStream(); // or could call dac.abortStream();
    }
  }

cleanup:
  if (dac.isStreamOpen())
    dac.closeStream();
  free(data);

  return 0;
}

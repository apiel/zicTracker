## TODO

Make grid UI more fancy (no need to stick to this text base UI)
- bring 3 grid view to one
- make pattern preview as background
- only display used clip and give the possibility to add more clip


Create 4 extra track, called MIDI_1 MIDI_2...
By default they are assigned to the channel, but can use a patch js file to change the channel
And send CC messages. Preset would also be available...
Patch, could even execute a bash script, but we might want execute it only once, to not run multiple
time the same? Or should it be a gloabl bash script that run only on start....?

Use sunvox synth engine
- https://warmplace.ru/soft/sunvox/sunvox_lib.php
- https://github.com/bohwaz/sunvox
- https://github.com/metrasynth/sunvox-guide

Synth, should there even be an envelop? Enverything could be wavetable base, using LFO as an envelop,
where one specific point could be sustain...
Should LFO be morphable? If not, let's just close te file...

For the moment, synth is single OSC, one day make dual OSC?

Make each track assignable to one type of synth engine and must stick to it (pd, internal synth, sample player)
OR should tracks be dedicated to a specific synth engine?

Need to make an LFO in memory

https://github.com/chriskiefer/OpenFrameworks-Supercollider-Host
https://github.com/chriskiefer/OpenFrameworks-Supercollider-Host/blob/master/src/embeddedSC.cpp
https://github.com/supercollider/SuperColliderAU

There could be only one instance of PD accessible from anytrack,
libpd can load multiple patches at the same, but using the same output stream.
We would therefor need to have a pd mixer, where each patch send their output to...
Each track would use a specific midi channel and we would have to find a way to set midi
channel for given patch and track output...
or use https://github.com/TheTechnobear/Orac ?
// https://puredata.info/docs/tutorials/TipsAndTricks#undocumented-pd-internal-messages

Use supercollidier or custom synth...
Custom synth can be patched using JS, so no need of fancy c++ logic

Should there be a bootstrap js file? set per project

App_Synth could have multiple layout using JS: a normal synth layout, but could also be a kick layout
However, for kick, we might need a more complex envelop generator or to provide a way to create a custom LFO waveshap that would run only once....

SDL audio record:
- https://lazyfoo.net/tutorials/SDL/34_audio_recording/index.php


Controller:
- https://gamesx.com/wiki/doku.php?id=controls:nes_snes_controller
- https://bitluni.net/gamepad
- https://github.com/bitluni/ArduinoGameController/blob/master/GameControllerExample/GameControllers.h


in pattern, we need to be able to add randomize feature!!

- we might be able to use 8 track?

- use TypeScript for JS file so we can define available type for the UI!!

- Audio usb host to mixer, so we could route mc-101 and circuit track audio into zic mixer (eventually add effect)

- doc!!  

- use supercollider (from JS file, instead to load PD file)

- vital in headless mode!! `./vital --headless` and only use macro from preset

- clap plugin ? https://github.com/free-audio/clap
- /usr/lib/lv2
    - https://github.com/zynthian/zynthian-sys/issues/200

- fix audio mixer
- fix wavetable from vital

- sort files for .wav

- effect: distortion, bitcrusher... waveshaper: overdrive, distortion, fuzz, clipping, expo converters, phase inversion
    - https://books.google.at/books?id=v0ulUYdhgXYC&pg=PA497&lpg=PA497&dq=Waveshaper+c%2B%2B+example&source=bl&ots=WVmstQmof2&sig=ACfU3U0LdpOVCUCtMP_zlnzEaXAZkCI-Qg&hl=en&sa=X&ved=2ahUKEwi31rmXisz5AhVW_bsIHRkhDM8Q6AF6BAgiEAM#v=onepage&q=Waveshaper%20c%2B%2B%20example&f=false

- save project!!!
    - save & restore
    make a copy of last state and keep auto "saving". Every change are saved in temp state, if click restore, restore to saved state.
    If click save, create new state that can be restored...

- conditional mute for x bar

- step probability

- midi tracks

- Fix / Implement more filter https://www.youtube.com/watch?v=XVOdqJy-Rfg

- think about clip system like mc-101 to play track more in a live way... :-)
    should this be combine to the track sequencer, directly usable within it
    or should it be another screen, where either using one or the other?? ^^
    -> maybe there should be one more button to force to jump to next pattern in seq

- in sequencer, give possibility to return to -1 -2 -3 -4 -5 pattern, jump back to previous pattern

- sampler:
    - should be able to record sound from tracks into a new sample
    - should be able to record sound from input channel (using usb-c sound card)
    - we could edit sample (set start/end) of selected sample

- audio input mixer

- doc within app
    -> base on markdown? or just JS...

- drum/kick engine

- fully use keyboard --> be able to map more key than just A and B, this can also be usefull for midi controller
    At least be able to map midi controller!

- create a pcb keyboard
    - give possibility to edit sound of the 4 tracks at the same time
    - ...
    - for the moment lets just use craft synth as midi controller

- be able to save project / pat / samples on github repo

- step component (conditional step)

- make graph using https://en.wikipedia.org/wiki/Braille_ASCII but then need to update font...

- what about this https://sites.google.com/site/musicgapi/technical-documents/wav-file-format#inst

- vital wavetable seem to use meta_data.set("clm ", "<!>2048 20000000 wavetable (vital.audio)");
    as riff metadata, maybe we could also try to support this in zic/zic_file_wav.h line 37
    in addition of ZICW

- amsynth
  can we do something with `AMSYNTH_NO_GUI=true amsynth` or `amsynth -x`
  then connect keyboard with `aconnect 'MPK mini':0 'amsynth':0`
  however we could send midi command from zictracker or maybe even use amsynth as a plugin...
  https://github.com/amsynth/amsynth/wiki/FAQs#can-i-run-amsynth-without-the-gui

- helm what s about https://github.com/mtytel/helm ? `helm --headless`

- synthv1 with no-gui option??

- output sound over usb

- info bull 
    only work with SDL display (so not in text mode)

https://github.com/free-audio/clap

## Synth lib

- https://github.com/TonicAudio/Tonic
- https://github.com/thestk/stk
- https://github.com/LancePutnam/Gamma

## RPi

Might want to use an HDMI audio convertor:
- https://www.amazon.de/dp/B09JLSBWDN/
- https://www.amazon.de/dp/B092VR953Z/
- https://www.amazon.de/-/en/gp/product/B07QLTDDHT/

However, would need to first get HDMI audio working...

Or use a DAC hat but a bit annoying to setup:
- https://www.amazon.de/-/en/gp/product/B07R8M3XFQ/

## AE

digital kick
- https://www.youtube.com/watch?v=A_Bv5Ad-Cy8
- https://www.youtube.com/watch?v=MK4ARnpljZg

## TODO

Make each track assignable to one type of synth engine and must stick to it (pd, internal synth, sample player)

DAC Hat Raspberry Pi I2S input (LRCK - GPIO19, BCK - GPIO18, DATA - GPIO21, GND)

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

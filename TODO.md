## TODO

- doc!!

- vital in headless mode!! `./vital --headless` 

- fix audio mixer
- fix wavetable from vital

- poly instrument, since pattern support multiple instrument playing at once
    we should do some like:
    - if instrument A set to wavetable (or sample once sample can be pitched)
    - and B set to poly, then instrument B replicate instrument A

- sort files for .wav

- effect: distortion, bitcrusher... waveshaper: overdrive, distortion, fuzz, clipping, expo converters, phase inversion
    - https://books.google.at/books?id=v0ulUYdhgXYC&pg=PA497&lpg=PA497&dq=Waveshaper+c%2B%2B+example&source=bl&ots=WVmstQmof2&sig=ACfU3U0LdpOVCUCtMP_zlnzEaXAZkCI-Qg&hl=en&sa=X&ved=2ahUKEwi31rmXisz5AhVW_bsIHRkhDM8Q6AF6BAgiEAM#v=onepage&q=Waveshaper%20c%2B%2B%20example&f=false
    
- assignable modulation to (1 or more? -> polyphony) track or instrument for selectable param:
    - 10? envelop 
    - 5? LFO
    - we could create wavetable to apply either on 1 or n steps
    - maybe there would be a way to make no difference between envelop and LFO, just need to find a good way to apply a modulation for a specific phase
    - SHOULD modulation be assignable or should it be fixed per synth? Maybe easier to have it fix per synth...

- save project!!!
    - save & restore
    make a copy of last state and keep auto "saving". Every change are saved in temp state, if click restore, restore to saved state.
    If click save, create new state that can be restored...

- use craft synth as midi controller

- midi tracks

- Fix / Implement more filter https://www.youtube.com/watch?v=XVOdqJy-Rfg

- think about clip system like mc-101 to play track more in a live way... :-)
    should this be combine to the track sequencer, directly usable within it
    or should it be another screen, where either using one or the other?? ^^
    -> maybe there should be one more button to force to jump to next pattern in seq

- in sequencer, give possibility to return to -1 -2 -3 -4 -5 pattern, jump back to previous pattern

- App_View_InstrumentWavetable select end start if sample

- sampler:
    - should be able to record sound from tracks into a new sample
    - should be able to record sound from input channel (using usb-c sound card)
    - we could edit sample (set start/end) of selected sample

- audio input mixer

- doc within app
    -> base on markdown? or simplified one using a converter...

- Use soundfont as alternative synth engine to wav table/sample
    - sf2 is work in progress in zic_file_soundfont.h. However still have a lot to do, see comment in top of the file
    - sf2 is too complexe, maybe we can think about a more simple alternative: 
        a big wav file with one chunk per note (a bit like the morphable wavetable)
        ZICK 0060 268945
        ZICK 0062 368945
        ...
        or ZKC4 268945
           ZKD4 368945
        it might a way to also define looping point

- drum/kick engine

- Use audio block processing instead of indiviual sample processing?? maybe :p

- should snapshot system not be base on rendered view but on custom format so changing the UI doesnt impact the snapshot?

- should rename all those synth to instrument

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

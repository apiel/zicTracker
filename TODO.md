## TODO

- doc!!

- save project!!!
    - name
    - save & restore
    make a copy of last state and keep auto "saving". Every change are saved in temp state, if click restore, restore to saved state.
    If click save, create new state that can be restored...


- create a pcb keyboard
    - give possibility to edit sound of the 4 tracks at the same time
    - ...

- effect: distortion, bitcrusher... waveshaper: overdrive, distortion, fuzz, clipping, expo converters, phase inversion
    - https://books.google.at/books?id=v0ulUYdhgXYC&pg=PA497&lpg=PA497&dq=Waveshaper+c%2B%2B+example&source=bl&ots=WVmstQmof2&sig=ACfU3U0LdpOVCUCtMP_zlnzEaXAZkCI-Qg&hl=en&sa=X&ved=2ahUKEwi31rmXisz5AhVW_bsIHRkhDM8Q6AF6BAgiEAM#v=onepage&q=Waveshaper%20c%2B%2B%20example&f=false
    
- assignable modulation to (1 or more? -> polyphony) track or instrument for selectable param:
    - 10? envelop 
    - 5? LFO

- Fix / Implement more filter https://www.youtube.com/watch?v=XVOdqJy-Rfg

- think about clip system like mc-101 to play track more in a live way... :-)
    should this be combine to the track sequencer, directly usable within it
    or should it be another screen, where either using one or the other?? ^^
    -> maybe there should be one more button to force to jump to next pattern in seq

- in sequencer, give possibility to return to -1 -2 -3 -4 -5 pattern, jump back to previous pattern

- drum/kick engine

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

- sampler

- polyphony
    - should allow to play 3 notes at once
    - in sample mode, it should allow to play 3 different samples at once?
    - could we even think so mixup sample and wavetable?
    - maybe the list of instrument should not be specific per track and then we could assign the instrument to the notes...


- App_View_InstrumentWavetable select end start if sample

- Use audio block processing instead of indiviual sample processing?? maybe :p

- fully use keyboard --> be able to map more key than just A and B, this can also be usefull for midi controller
    At least be able to map midi controller!

- be able to save project / pat / samples on github repo

- step component (conditional step)

- make graph using https://en.wikipedia.org/wiki/Braille_ASCII but then need to update font...

- sampler:
    - should be able to record sound from tracks into a new sample
    - should be able to record sound from input channel (using usb-c sound card)
    - we could edit sample (set start/end) of selected sample
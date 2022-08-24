## TODO

- doc!!

- save project!!!

- create a pcb keyboard
    - give possibility to edit sound of the 4 tracks at the same time
    - ...

- effect: distortion, bitcrusher... waveshaper: overdrive, distortion, fuzz, clipping, expo converters, phase inversion
    - https://books.google.at/books?id=v0ulUYdhgXYC&pg=PA497&lpg=PA497&dq=Waveshaper+c%2B%2B+example&source=bl&ots=WVmstQmof2&sig=ACfU3U0LdpOVCUCtMP_zlnzEaXAZkCI-Qg&hl=en&sa=X&ved=2ahUKEwi31rmXisz5AhVW_bsIHRkhDM8Q6AF6BAgiEAM#v=onepage&q=Waveshaper%20c%2B%2B%20example&f=false
    
- assignable modulation to (1 or more? -> polyphony) track or instrument for selectable param:
    - 10? envelop 
    - 5? LFO

- Fix / Implement more filter https://www.youtube.com/watch?v=XVOdqJy-Rfg

- Use soundfont as alternative synth engine to wav table/sample
            Maybe even use https://github.com/schellingb/TinySoundFont/blob/master/examples/example1.c

- sampler

- think about clip system like mc-101 to play track more in a live way... :-)
    should this be combine to the track sequencer, directly usable within it
    or should it be another screen, where either using one or the other?? ^^
    -> maybe there should be one more button to force to jump to next pattern in seq

- drum/kick engine

- in sequencer, give possibility to return to -1 -2 -3 -4 -5 pattern, jump back to previous pattern

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
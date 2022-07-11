
## Project

- name
- save & restore
make a copy of last state and keep auto "saving". Every change are saved in temp state, if click restore, restore to saved state.
If click save, create new state that can be restored...

## Pattern

TODO

A pattern, 32 - 64? step note for any of the 16 instrument

## Tracks

TODO

Tracks can be synched together to make polyphonic instument, so one track is a copy of the other, just notes differ.
Keyboard can also be link to multiple track at once, to play some harmonic.

## Instrument

TODO

Sample and waveform are both using wav file from the SD card.

- Wavetable
- Sample (only one pitch for the moment)
- Multi-samples (would allow to have multiples sound in one sample, each sound correspond to a note, either the same sound with different pitch or a sound kit e.g. drumkit)

## Notes/todo

- track table view should have header
- 1 volumn per track
- 1 filter per track
- multiple track can be assigned to master keyboard
- 1 filter per instrument
- 1 envelop per instrument
- assignable modulation to (1 or more? -> polyphony) track or instrument for selectable param:
    - 10? envelop 
    - 5? LFO

- make graph using https://en.wikipedia.org/wiki/Braille_ASCII but then need to update font...

## Toolchain
### Miyoo mini

Need to use buildroot https://github.com/shauninman/miyoomini-toolchain-buildroot/

```sh
make shell
cd ~/buildroot
make menuconfig
make sdk
cd ~
./package-toolchain.sh
exit
```

### anbernic rg 280v

Might need to install gcc-multilib, gcc-multilib and maybe some other stuff for x64 bit computer
Install toolchain opendingux-gcw0-toolchain.2014-08-20
maybe there: http://www.gcw-zero.com/develop
or somewhere there: http://od.abstraction.se/opendingux/
                    http://od.abstraction.se/opendingux/toolchain/
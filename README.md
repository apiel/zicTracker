## Pattern

TODO

A pattern, 32 - 64? step note for any of the 16 instrument

## Tracks

TODO

Tracks can be synched together to make polyphonic instument, so one track is a copy of the other, just notes differ.
Keyboard can also be link to multiple track at once, to play some harmonic.

## Instrument

TODO

Sample and waveform are both using wav file from the SD card. They are treated exactly the same way, just behaving slghtly different. Sample will play only once, where the waveform will loop over.

Both have:
- start and len
- ~2 env, assignable -> amplitude, pitch, filter, ifx~
- ~1 assignable lfo -> amplitude, pitch, filter, ifx, morph? (or should there only be 1-4 lfo in total?)~
- 1 filter
- 1 ifx

Waveform:
- will most likely be much shorted but doesnt have to, also can do morphing over

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
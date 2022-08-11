## TODO

- use https://waveeditonline.com/ to load morphable wavetables

- SHould we use audio block processing instead of indiviual sample processing?? maybe :p

- Implement more filter https://www.youtube.com/watch?v=XVOdqJy-Rfg

- // FIXME modulo, dont use motulo but comparison

- Use soundfont as alternative synth engine to wav table/sample
            Maybe even use https://github.com/schellingb/TinySoundFont/blob/master/examples/example1.c

- fully use keyboard --> be able to map more key than just A and B, this can also be usefull for midi controller
    At least be able to map midi controller!

## Project

- name
- save & restore
make a copy of last state and keep auto "saving". Every change are saved in temp state, if click restore, restore to saved state.
If click save, create new state that can be restored...

## Pattern

TODO

A pattern, 32 - 64? step note for any of the 16 instrument
conditional steps / step modification of time

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
### anbernic rg 280v

Might need to install gcc-multilib, gcc-multilib and maybe some other stuff for x64 bit computer
Install toolchain opendingux-gcw0-toolchain.2014-08-20
maybe there: http://www.gcw-zero.com/develop
or somewhere there: http://od.abstraction.se/opendingux/
                    http://od.abstraction.se/opendingux/toolchain/

### anbernic rg351

https://github.com/christianhaitian/arkos/wiki/Building

To create debian based chroots in a Linux environment.
These instructions are based on a Ubuntu 16 or newer install or VM.
install Prereqs:

```sh
sudo apt update
sudo apt install -y build-essential debootstrap binfmt-support qemu-user-static
```

Then install armhf and arm64 chroots:

sudo qemu-debootstrap --arch arm64 buster /mnt/data/arm64 http://deb.debian.org/debian/

Get into chroots:

For 64 bit Arm environment:
```sh
sudo chroot /mnt/data/arm64/
```
or create a Arm64 shortcut on the desktop gui and click on Arm64 shortcut on desktop


Helpful tools to install in both environments for RK3326 app builds

```sh
apt -y install build-essential git wget libdrm-dev python3 python3-pip python3-setuptools python3-wheel ninja-build libopenal-dev premake4 autoconf libevdev-dev ffmpeg libsnappy-dev libboost-tools-dev magics++ libboost-thread-dev libboost-all-dev pkg-config zlib1g-dev libpng-dev libsdl2-dev clang cmake cmake-data libarchive13 libcurl4 libfreetype6-dev libjsoncpp1 librhash0 libuv1 mercurial mercurial-common libgbm-dev libsdl2-ttf-2.0-0 libsdl2-ttf-dev
```

or using docker https://github.com/mica-angeli/retrooz_dev_docker

# Zic Tracker

Zic Tracker is a groovebox engine base on samples and wavetables. It is intended to work on handheld linux console like anbernic rg351p (work as well on Linux desktop for development purpose). The user interface is using SDL2, inspired by some existing trackers like LSDJ, LittleGPTracker or M8.

## Navigation

Menu button (B), is used to display the menu tree.
Edit button (A), is used to change the selected value.
Menu + Edit button (A + B), is to play/pause the sequencer.

## Sequencer

![sequencer](https://github.com/apiel/zicTracker/blob/main/screenshot/seq.png?raw=true)

The sequencer view is used to chain pattern. It has 4 columns, 1 for each tracks. Each field is a combination of 3 elements, the first 2 char represent the pattern, the seconds 2 char represent the detune value (+0), the third 2 char represent the action to do for this given filed. If ">" is in front of the field, it mean that it is currently playing. If "\*" is front of the field, it mean that the sequencer is paused on this element.

### Detune

Detune, will allow to increase or decrease all the notes of the pattern by semitone. It goes from -Z to +0 to +Z, where +Z correspond to +35 semitone as we used letter in order to use less space on the screen (+1, +2,..., +9, +A, +B,..., +Y, +Z).

### Actions

- x0 will repeat the pattern for ever
- x1 will play the pattern only once and then go to the next row
- x2 will play the pattern twice and then go to the next row
- x3 will play the pattern 3 times and then go to the next row
- x4 will play the pattern 4 times and then go to the next row
- x5 will play the pattern 5 times and then go to the next row
- ! will stop after playing the pattern
- << go back to start

## Project

TBD.

## Pattern

![sequencer](https://github.com/apiel/zicTracker/blob/main/screenshot/pat.png?raw=true)

There is 255 patterns per project from 01 to FF (using hexadecimal number to represent the ID of the pattern). A pattern can be up of 64 steps.
There is 4 assignable notes per step. Those notes can be assign to one instrument for polyphony or to different instruments.

`E_3⠾1⤸`

The first 3 char `E_3` is the note. The 4th char is the velocity `⠾`. The 5th char is the conditional to play the pattern. The 6th char `⤸` is the tie to the next step.

### Step condition

- 1 will play on every step
- 2 will play every second step
- 3 will play every third step
- ...

## Tracks

TBD.

## Instrument

![sequencer](https://github.com/apiel/zicTracker/blob/main/screenshot/inst.png?raw=true)

TBD.

Sample and waveform are both using wav file from the SD card.

- Wavetable
- Morphable wavetables using https://waveeditonline.com/ and https://synthtech.com/waveedit
- Sample (only one pitch)
- Multi-samples with soundfont (sf2)

## Sampler

TBD.

## Linux Installation

Need to install libpd.

```sh
git clone --recurse-submodules https://github.com/libpd/libpd.git
cd libpd
make MULTI=true
cd ..
```

```sh
apt-get install libsdl2-dev
# in the same folder clone both repo
git clone https://github.com/apiel/zic.git
git clone https://github.com/apiel/zicTracker.git
cd zicTracker/linuxSDL
make
```

Need to download https://duktape.org/download.html (and not clone got repo)
Might want to use shared library for duktape to make compilation quicker: `make -f Makefile.sharedlibrary`.
For rg351, need to chroot...

Need to install https://github.com/thestk/rtmidi

Or install libs??

```sh
apt-get install libsdl2-dev libsndfile1 libduktape207 duktape-dev librtmidi-dev librtmidi6
```

Use `ldconfig -p | grep libduktape` to see what is already installed.

## Toolchain

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

### anbernic rg280v

(rg280v seem to not be powerful enough to run Zic Tracker)

Might need to install gcc-multilib, gcc-multilib and maybe some other stuff for x64 bit computer
Install toolchain opendingux-gcw0-toolchain.2014-08-20
maybe there: http://www.gcw-zero.com/develop
or somewhere there: http://od.abstraction.se/opendingux/
http://od.abstraction.se/opendingux/toolchain/

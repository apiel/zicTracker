# Zic Tracker

Zic Tracker is a groovebox engine base on samples and wavetables. It is intended to work on handheld linux console like anbernic rg351p (work as well on Linux desktop for development purpose). The user interface is using SDL2, inspired by some existing trackers like LSDJ, LittleGPTracker or M8.

## Navigation

Menu button (B), is used to display the menu tree.
Edit button (A), is used to change the selected value.
Menu + Edit button (A + B), is to play/pause the sequencer.

## Project

- name
- save & restore
make a copy of last state and keep auto "saving". Every change are saved in temp state, if click restore, restore to saved state.
If click save, create new state that can be restored...

## Pattern

TBD.

## Tracks

TBD.
## Instrument

TBD.

Sample and waveform are both using wav file from the SD card.

- Wavetable
- Morphable wavetables using https://waveeditonline.com/ and https://synthtech.com/waveedit
- Sample (only one pitch)
- Multi-samples with soundfont (sf2)

## Sampler

TBD.

## Linux Installation

```sh
apt-get install libsdl2-dev
# in the same folder clone both repo
git clone https://github.com/apiel/zic.git
git clone https://github.com/apiel/zicTracker.git
cd zicTracker/linuxSDL
make
```

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
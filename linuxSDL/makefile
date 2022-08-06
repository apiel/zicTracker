# for FTP EOF
.ONESHELL:

# zic lib folder
ZIC_LIB=../../zic

# default
SDL_CONFIG=sdl2-config
CC=g++ -o zicTracker
ZIC_TARGET=0
#ZIC_SDL_MIXER=-l SDL2_mixer -DZIC_SDL_MIXER=1

# Open Dingux
OPENDINGUX_ROOT=/opt/gcw0-toolchain
OPENDINGUX_CC=$(OPENDINGUX_ROOT)/usr/bin/mipsel-gcw0-linux-uclibc-g++ -o zicTracker.elf -std=c++11
OPENDINGUX_SDL_CONFIG=$(OPENDINGUX_ROOT)/usr/mipsel-gcw0-linux-uclibc/sysroot/usr/bin/sdl2-config
OPENDINGUX_TARGET=1

#rg351
# https://github.com/christianhaitian/arkos/wiki/Building
# or https://github.com/mica-angeli/retrooz_dev_docker

linux: build run

opendingux:
	make ROOT=$(OPENDINGUX_ROOT) CC="$(OPENDINGUX_CC)" SDL_CONFIG="$(OPENDINGUX_SDL_CONFIG)" ZIC_TARGET=$(OPENDINGUX_TARGET) build opk

build:
	$(CC) -Wall zicTracker.cpp -fopenmp -Wno-narrowing -I$(ZIC_LIB) `$(SDL_CONFIG) --cflags --libs` -DZIC_TARGET=$(ZIC_TARGET) $(ZIC_SDL_MIXER)

# -fopenmp for soundTouch "warning: ignoring #pragma omp parallel"
# -Wno-narrowing for font.h:62:56: warning: narrowing conversion of ‘(8 * ((int)size))’ from ‘int’ to ‘Uint16’ {aka ‘short unsigned int’} inside { } [-Wnarrowing]

run:
	./zicTracker

opk:
	./opk.sh
	ftp -n <<EOF
	open 10.1.1.2
	user root none
	cd apps
	put zicTracker.opk
	EOF

# echo 'opkrun /media/data/apps/zicTracker.opk' | telnet 10.1.1.2
# echo 'opkrun /media/data/apps/zicTracker.opk' | nc 10.1.1.2 23

# for FTP EOF
.ONESHELL:

# zic lib folder
ZIC_LIB=../../zic

# default
SDL_CONFIG=sdl2-config
CC=g++ -o zicTracker
ZIC_TARGET=0
ZIC_SDL2=true
ZIC_SDL_MIXER=SDL2_mixer

# Open Dingux
OPENDINGUX_ROOT=/opt/gcw0-toolchain
OPENDINGUX_CC=$(OPENDINGUX_ROOT)/usr/bin/mipsel-gcw0-linux-uclibc-g++ -o zicTracker.elf -std=c++11
OPENDINGUX_SDL_CONFIG=$(OPENDINGUX_ROOT)/usr/mipsel-gcw0-linux-uclibc/sysroot/usr/bin/sdl2-config
OPENDINGUX_TARGET=1

# Miyoo
MIYOO_ROOT=/opt/miyoomini-toolchain
MIYOO_CC=$(MIYOO_ROOT)/usr/bin/arm-linux-gnueabihf-g++ -o zicTrackerMiyoo
MIYOO_SDL_CONFIG=$(MIYOO_ROOT)/usr/arm-linux-gnueabihf/sysroot/usr/bin/sdl2-config
MIYOO_SDL12_CONFIG=$(MIYOO_ROOT)/usr/arm-linux-gnueabihf/sysroot/usr/bin/sdl-config
MIYOO_TARGET=2

linux: build run

miyoo2:
	make ROOT=$(MIYOO_ROOT) CC="$(MIYOO_CC)-2" SDL_CONFIG="$(MIYOO_SDL_CONFIG)" ZIC_TARGET=$(MIYOO_TARGET) build

miyoo1.2:
	make ROOT=$(MIYOO_ROOT) CC="$(MIYOO_CC)-1.2" SDL_CONFIG="$(MIYOO_SDL12_CONFIG)" ZIC_TARGET=$(MIYOO_TARGET) ZIC_SDL2=false ZIC_SDL_MIXER=SDL_mixer build serial_send

opendingux:
	make ROOT=$(OPENDINGUX_ROOT) CC="$(OPENDINGUX_CC)" SDL_CONFIG="$(OPENDINGUX_SDL_CONFIG)" ZIC_TARGET=$(OPENDINGUX_TARGET) build opk

build:
	$(CC) -Wall zicTracker.cpp -fopenmp -Wno-narrowing -I$(ZIC_LIB) -DZIC_SDL2=$(ZIC_SDL2) `$(SDL_CONFIG) --cflags --libs` -DZIC_TARGET=$(ZIC_TARGET) -l$(ZIC_SDL_MIXER)

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

serial_send:
	cd serial
	node main.js

# for FTP EOF
.ONESHELL:

# zic lib folder
ZIC_LIB=../../zic
ZIC_APP_CORE_LIB=../../zicAppCore

# default
SDL_CONFIG=sdl2-config
CC=g++ -o zicTracker
ZIC_TARGET=0
#ZIC_SDL_MIXER=-l SDL2_mixer -DZIC_SDL_MIXER=1
LV2=`pkg-config --libs lilv-0` -I/usr/include/lilv-0

# Open Dingux
OPENDINGUX_ROOT=/opt/gcw0-toolchain
OPENDINGUX_CC=$(OPENDINGUX_ROOT)/usr/bin/mipsel-gcw0-linux-uclibc-g++ -o zicTracker.elf -std=c++11
OPENDINGUX_SDL_CONFIG=$(OPENDINGUX_ROOT)/usr/mipsel-gcw0-linux-uclibc/sysroot/usr/bin/sdl2-config
OPENDINGUX_TARGET=1

RG351_IP=192.168.1.111

#rg351
# https://github.com/christianhaitian/arkos/wiki/Building
# or https://github.com/mica-angeli/retrooz_dev_docker

linux: build run

opendingux:
	make ROOT=$(OPENDINGUX_ROOT) CC="$(OPENDINGUX_CC)" SDL_CONFIG="$(OPENDINGUX_SDL_CONFIG)" ZIC_TARGET=$(OPENDINGUX_TARGET) build opk

rg351:
	make rg351Chroot rg351SCP rg351Log
	
rg351Chroot:
	sudo mount --bind /home/alex/Music/zic /mnt/data/arm64/home/alex/zic
	sudo chroot /mnt/data/arm64/ /bin/bash -c "cd /home/alex/zic/zicTracker/linuxSDL && make rg351Chrooted"

# or manualy: 
# sudo chroot /mnt/data/arm64/
# and then run `make rg351Chrooted` in the chroot

rg351Chrooted:
	make CC="g++ -o zicTrackerRG351" build

rg351SCP:
	sshpass -p "ark" scp zicTrackerRG351 ark@$(RG351_IP):/roms/ports/zic/zicTracker

rg351Kill:
	sshpass -p "ark" ssh ark@$(RG351_IP) /roms/ports/zic/kill.sh

rg351Log:
	sshpass -p "ark" ssh ark@$(RG351_IP) tail -f /roms/ports/zic/zic.log

build:
	$(CC) -Wall zicTracker.cpp -fopenmp -Wno-narrowing -I$(ZIC_LIB) -I$(ZIC_APP_CORE_LIB) `$(SDL_CONFIG) --cflags --libs` $(LV2)  -DZIC_TARGET=$(ZIC_TARGET) $(ZIC_SDL_MIXER)

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

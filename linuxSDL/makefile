# zic lib folder
ZIC_LIB=../../zic

# default
SDL2_CONFIG=`sdl2-config --cflags --libs`
CC=g++ -o zicServer

# Open Dingux
#OPENDINGUX_ROOT=/home/alex/Music/console/opendingux-gcw0-toolchain.2014-08-20/gcw0-toolchain
OPENDINGUX_ROOT=/opt/gcw0-toolchain
OPENDINGUX_CC=$(OPENDINGUX_ROOT)/usr/bin/mipsel-gcw0-linux-uclibc-g++ -o zicServer.elf -std=c++11
OPENDINGUX_SDL2_CONFIG=`$(OPENDINGUX_ROOT)/usr/mipsel-gcw0-linux-uclibc/sysroot/usr/bin/sdl2-config --cflags --libs`

linux: build run

opendingux:
	make CC="$(OPENDINGUX_CC)" SDL2_CONFIG="$(OPENDINGUX_SDL2_CONFIG)"

build:
	$(CC) -Wall -D__LINUX_PULSE__ zicServer.cpp -I$(ZIC_LIB) $(SDL2_CONFIG)

run:
	./zicServer

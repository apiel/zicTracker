# zic lib folder
ZIC_LIB=../../zic

# default
SDL_CONFIG=
CC=g++ -o zicServer

# Miyoo
MIYOO_ROOT=/opt/miyoomini-toolchain
MIYOO_CC=$(MIYOO_ROOT)/usr/bin/arm-linux-gnueabihf-g++ -o zicServerMiyoo
MIYOO_SDL_CONFIG=$(MIYOO_ROOT)/usr/arm-linux-gnueabihf/sysroot/usr/bin/

linux: build run

miyoo:
	make ROOT=$(MIYOO_ROOT) CC="$(MIYOO_CC)" SDL_CONFIG="$(MIYOO_SDL_CONFIG)" build

build:
	$(CC) -Wall zicServer.cpp -I$(ZIC_LIB) `$(SDL_CONFIG)sdl-config --cflags --libs`

run:
	./zicServer

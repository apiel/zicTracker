# for FTP EOF
.ONESHELL:

# zic lib folder
ZIC_LIB=../../zic

# default
SDL2_CONFIG=
CC=g++ -o zicServer
ZIC_TARGET=0

# Open Dingux
OPENDINGUX_ROOT=/opt/gcw0-toolchain
OPENDINGUX_CC=$(OPENDINGUX_ROOT)/usr/bin/mipsel-gcw0-linux-uclibc-g++ -o zicServer.elf -std=c++11
OPENDINGUX_SDL2_CONFIG=$(OPENDINGUX_ROOT)/usr/mipsel-gcw0-linux-uclibc/sysroot/usr/bin/
OPENDINGUX_TARGET=1

# Miyoo
MIYOO_ROOT=/opt/miyoomini-toolchain
MIYOO_CC=$(MIYOO_ROOT)/usr/bin/arm-linux-gnueabihf-g++ -o zicServerMiyoo
MIYOO_SDL2_CONFIG=$(MIYOO_ROOT)/usr/arm-linux-gnueabihf/sysroot/usr/bin/

linux: build run

miyoo:
	make ROOT=$(MIYOO_ROOT) CC="$(MIYOO_CC)" SDL2_CONFIG="$(MIYOO_SDL2_CONFIG)" build

opendingux:
	make ROOT=$(OPENDINGUX_ROOT) CC="$(OPENDINGUX_CC)" SDL2_CONFIG="$(OPENDINGUX_SDL2_CONFIG)" ZIC_TARGET=$(OPENDINGUX_TARGET) build opk

build:
	$(CC) -Wall zicServer.cpp -I$(ZIC_LIB) `$(SDL2_CONFIG)sdl2-config --cflags --libs` -DZIC_TARGET=$(ZIC_TARGET)

run:
	./zicServer

opk:
	./opk.sh
	ftp -n <<EOF
	open 10.1.1.2
	user root none
	cd apps
	put zicServer.opk
	EOF

# echo 'opkrun /media/data/apps/zicServer.opk' | telnet 10.1.1.2
# echo 'opkrun /media/data/apps/zicServer.opk' | nc 10.1.1.2 23

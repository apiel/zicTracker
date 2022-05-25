# zic lib folder
ZIC_LIB=../../zic

all: build run

build:
	g++ -Wall -D__LINUX_PULSE__ -o zicServer zicServer.cpp -I$(ZIC_LIB) `sdl2-config --cflags --libs`

run:
	./zicServer

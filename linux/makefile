# zic lib folder
ZIC_LIB=../../zic

all: build run

build:
	g++ -Wall -D__LINUX_PULSE__ -o zicServer zicServer.cpp RtAudio.cpp -lpthread -lpulse-simple -lpulse -I$(ZIC_LIB)

run:
	./zicServer

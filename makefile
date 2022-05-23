all: build run

build:
	g++ -Wall -D__LINUX_PULSE__ -o zicServer zicServer.cpp RtAudio.cpp -lpthread -lpulse-simple -lpulse -I../zic

run:
	./zicServer

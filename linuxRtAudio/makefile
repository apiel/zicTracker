# zic lib folder
ZIC_LIB=../../zic

all: build run

build:
	/opt/miyoomini-toolchain/usr/bin/arm-linux-gnueabihf-g++ -Wall -o zicServerMiyoo -D__LINUX_ALSA__ zicServer.cpp RtAudio.cpp -lpthread -lasound -I$(ZIC_LIB)	-L/home/alex/Downloads/parasyte_20220125/parasyte/rootfs/usr/lib
#	g++ -Wall -D__LINUX_PULSE__ -o zicServer zicServer.cpp RtAudio.cpp -lpthread -lpulse-simple -lpulse -I$(ZIC_LIB)

run:
	./zicServer

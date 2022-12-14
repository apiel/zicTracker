# for FTP EOF
.ONESHELL:

# zic lib folder
ZIC_LIB=../../zic
ZIC_APP_CORE_LIB=../../zicAppCore

# default
CC=g++ -o zicTracker
ZIC_TARGET=0
# LV2=`pkg-config --libs lilv-0` -I/usr/include/lilv-0

# For libpd_arm.so, we should first chroot into the arm environment
# and then compile everything again, finally rename it as libpd_arm.so
# PUREDATA=-DPD -DHAVE_UNISTD_H -DUSEAPI_DUMMY ./libpd.so -I../../libpd/pure-data/src \
#          -I../../libpd/libpd_wrapper -I../../libpd/cpp
LIBPD_DIR=../../libpd
PUREDATA = -I$(LIBPD_DIR)/libpd_wrapper -I$(LIBPD_DIR)/libpd_wrapper/util \
           -I$(LIBPD_DIR)/pure-data/src -I$(LIBPD_DIR)/cpp \
		   ./libpd.so -DPDINSTANCE

#JSAMALGAM=../../jerryscript/amalgam
#JERRYSCRIPT = $(JSAMALGAM)/jerryscript.c $(JSAMALGAM)/jerryscript-port.c -I$(JSAMALGAM)/
JERRYSCRIPT = `pkg-config --cflags --libs libjerry-core libjerry-port libjerry-math`

# Build without shared lib
# DUKTAPE= -I../../duktape-2.7.0/src ../../duktape-2.7.0/src/duktape.c
# Build with shared lib
DUKTAPE= -I../../duktape-2.7.0/src ./libduktaped.so.207
JS_ENGINE=$(DUKTAPE)

RG351_IP=192.168.1.108

#rg351
# https://github.com/christianhaitian/arkos/wiki/Building
# or https://github.com/mica-angeli/retrooz_dev_docker

linux: puredata duk build run

# in case we use shared lib for duktape
duk:
	cp ../../duktape-2.7.0/libduktaped.so.207.20700 ./libduktaped.so.207

rg351Duk:
	cp ../../duktape-2.7.0/libduktaped.so.207.20700.arm ./libduktaped.so.207

puredata:
	cp ../../libpd/libs/libpd.so .

rg351Puredata:
	cp ../../libpd/libs/libpd_arm.so libpd.so

rg351:
	make rg351MountChroot rg351Next

rg351Next:
	make rg351Puredata rg351Duk rg351Chroot rg351SCP
	
rg351MountChroot:
	sudo mount --bind /home/alex/Music/zic /mnt/data/arm64/home/alex/zic

rg351Chroot:
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

rg351CatLog:
	sshpass -p "ark" ssh ark@$(RG351_IP) cat /roms/ports/zic/zic.log

rg351Project:
	sshpass -p "ark" ssh ark@$(RG351_IP) rm -rf /roms/ports/zic/projects
	sshpass -p "ark" scp -r projects/ ark@$(RG351_IP):/roms/ports/zic/projects

build:
	$(CC) -Wall zicTracker.cpp -fopenmp -Wno-narrowing -I$(ZIC_LIB) -I$(ZIC_APP_CORE_LIB) \
	`sdl2-config --cflags --libs` $(LV2)  -DZIC_TARGET=$(ZIC_TARGET) \
	 $(PUREDATA) $(JS_ENGINE)

# -fopenmp for soundTouch "warning: ignoring #pragma omp parallel"
# -Wno-narrowing for font.h:62:56: warning: narrowing conversion of ‘(8 * ((int)size))’ from ‘int’ to ‘Uint16’ {aka ‘short unsigned int’} inside { } [-Wnarrowing]

run:
	echo start zicTracker
	LD_LIBRARY_PATH=. ./zicTracker

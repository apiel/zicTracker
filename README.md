### Miyoo mini

download toolchain https://github.com/shauninman/miyoomini-toolchain-buildroot/releases
or build it from docker file
or maybe even apt-get install gcc-arm-linux-gnueabihf

git clone https://github.com/libsdl-org/SDL.git

Nope,

Need to use buildroot https://github.com/shauninman/miyoomini-toolchain-buildroot/

And enable SDL2 in the packages

```sh
make shell
cd ~/buildroot
make menuconfig
make sdk
cd ~
./package-toolchain.sh
exit
```

### anbernic rg 280v

Might need to install gcc-multilib, gcc-multilib and maybe some other stuff for x64 bit computer
Install toolchain opendingux-gcw0-toolchain.2014-08-20
maybe there: http://www.gcw-zero.com/develop
or somewhere there: http://od.abstraction.se/opendingux/
                    http://od.abstraction.se/opendingux/toolchain/
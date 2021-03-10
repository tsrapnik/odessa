# 0: odessa

## author
tsrapnik

## goal

odessa turns your raspberry pi into an audio effect processor, like stompbox for guitar. the idea is that you can easily create your own effects or adapt the interface to your own needs. the software runs bare metal (meaning without an operating system), so there are no problems with lag or long boot times.

despite being built for an audio effect processor, many parts of this project can be used as a framework for other raspberry pi bare metal projects.

at the moment of writing the project is far from completion. just a basic gui and a framework for the audio effects is available, no audio processing is done yet.
<!--todo: update status regularly.)-->

## references
below a nonexhaustive list of projects and blogs that i used as an inspiration or example for my code. do follow the links and check these projects and blogs out.
* [LdB-ECM](https://github.com/LdB-ECM/Raspberry-Pi)
* [rsta2](https://github.com/rsta2/circle)
* [dwelch67](https://github.com/dwelch67/raspberrypi)
* [PeterLemon](https://github.com/PeterLemon/RaspberryPi)
* [jaystation2](http://jaystation2.maisonikkoku.com/)
* [metebalci](https://metebalci.com/blog/bare-metal-rpi3-network-boot/)
* [raspberrypi](https://github.com/raspberrypi/firmware/wiki/)

## usage

### compile
todo: update compiler link.
this repository was originally built on a ubuntu machine with visual studio code, but it should run fine with linux flavours and editors. to build the project [make](https://www.gnu.org/software/make/), a bash terminal and an arm gcc compiler are needed. make and bash are preinstalled on most linux machines, on windows you'll have to google for an alternative. the project is compiled with [gcc-linaro-7.1.1-2017.08-x86_64_aarch64-elf](https://releases.linaro.org/components/toolchain/binaries/7.1-2017.08/aarch64-elf/). it is advised to use the same compiler (free downloadable), otherwise the binary might not work.

to compile the project the build.sh script should be modified, according to the instructions inside it. this mostly means defining the paths to the compiler and other recourses and choosing between a few options.

the most important option is the boot mode. you can boot from an sd card (fast boot time, ideal for running a finished program) or over the network (boots slower, but is more convenient while programming).

## todo: custom compiler used.
Building toolchain from source
You can build GNU cross-toolchain for the A-profile from sources using Linaro ABE (Advanced Build Environment) and provided ABE manifest files.

Below example shows how to build gcc-arm-aarch64-linux-gnu toolchain from sources using Linaro ABE build system.

Instructions
Clone ABE one of the URL below and checkout the stable branch (see Getting ABE):

$ git clone https://git.linaro.org/toolchain/abe.git
Create the build directory and change to it. Any name for the directory will work (see Building Toolchains With ABE):

$ mkdir build && cd build
Configure ABE (from the build directory):

$ ../abe/configure
And finally build toolchain (from the build directory):

$ ../abe/abe.sh --manifest gcc-linaro-7.4.1-2019.02-linux-manifest.txt  --build all

now open build_folder/snapshots/gcc.git~linaro-7.4-2019.02_rev_56ec6f6b99cc167ff0c2f8e1a2eed33b1edc85d4 which contains project. edit files (explain as in git diff). remove all gcc-**-.manifest files from /home/tsrapnik/stack/projects/shared_resources/compilers/gcc-linaro-7.4.1-2019.02-x86_64_aarch64-elf-build/builds/x86_64-unknown-linux-gnu/aarch64-elf to force gcc to get compiled again.

probably a better way to do this, but it was the first thing i found that worked.

$../abe/abe.sh --manifest gcc-linaro-7.4.1-2019.02-linux-manifest.txt --build all --disable update

#### boot from sd card
to boot from an sd card you of course need an sd card. put the card in your pc and add the correct path to this card in the build.sh file. we will now put the necessary boot files on the card with following commands. the chmod command needs to be done only once, it just tells linux build.sh is an executable script.

```
cd /path_to_project_folder/odessa
chmod +x build.sh
./build.sh boot
```

check the sd card. it should contain a few files which the pi uses for the primary boot stages.

now run the following command to actually compile the project and copy the bootable image also to the sd card. in visual studio code the command can by run by the shortcut ctrl+shift+b.

```
./build.sh
```

each time you run the script only the changes to the project are recompiled and the new image is copied to the sd card. to recompile everything run following command first.

```
./build.sh clean
```

#### boot from network

this is a little trickier. you will need to follow a few steps first before network booting is possible.

##### enable uart logging

this step is not necessary but very usefull. uart logging is enabled during the boot, so you can easily follow along and see where things go wrong if they do.

you need a some kind of uart connection betweeen your pc and the pi. a usb to uart dongle based on the cp2102 will do fine. connect ... to ...
\
todo: explain how to use dongle.

```
lsusb
...
screen /dev/ttyUSBx 115200
```
todo: bootcode.bin taken form debian image.
the bootcode.bin file which is used by the gpu on the pi as second bootstage can be modified to enable uart logging during boot stage. the bootcode.bin file provide in the project is already modified, so nothing needs to be done. if needed you can modify the bootcode.bin file yourself with following commands in a terminal. first command checks if the uart logging is enabled.

```
strings bootcode.bin | grep BOOT_UART
```

it will respond with `BOOT_UART=0` when disabled and `BOOT_UART=1` when enabled. to enable uart logging use following command.

```
sed -i -e "s/BOOT_UART=0/BOOT_UART=1/" bootcode.bin
```

##### enable network boot mode

you now need to enable network boot by setting a special register in the soc. firstly boot your raspberry pi with the regular debian distribution. then open a terminal and enter following command.

```
vcgencmd otp_dump | grep 17
```

if the response is `3020000a` (should be on a pi 3b+) network boot is already enabled. if the response is `1020000a` (should be on a pi 3b) network boot is disabled. to enable the network boot open the config.txt file.

```
sudo nano /boot/config.txt
```

add following line to the file and close with ctrl+x and confirm to save with Y.

```
program_usb_boot_mode=1
```

now reboot the pi. you should now get `3020000a` as response, confirming network boot is enabled. the register can be changed only once, so once done this cannot be undone, however you can still boot from sd card if needed. for the pi 3 todo: check if different boot options are well explained.

#### setup dhcp server

instructions are 

on linux (if virtualbox then bridged adapter promiscuous)

```
sudo tcpdump port bootpc (-v)
```

```
sudo apt install isc-dhcp-server
```

edit /etc/dhcp/dhcpd.conf

```
default-lease-time 600;
max-lease-time 7200;
ddns-update-style none;
authoritative;
log-facility local7;
subnet 192.168.1.0 netmask 255.255.255.0 {
  range 192.168.1.211 192.168.1.212;
  option routers 192.168.1.1;
  option domain-name-servers 8.8.8.8, 8.8.4.4;
  option tftp-server-name "192.168.1.37";
}
host odessa {
  hardware ethernet b8:27:eb:3e:7b:47;
  fixed-address 192.168.1.210;
}
```

```
sudo service isc-dhcp-server restart
```

todo: sometimes required to restart server with above command.

#### setup tftp server

```
sudo apt install tftpd
```

add file tftp to /etc/xinetd.d

```
service tftp
{
  protocol        = udp
  port            = 69
  socket_type     = dgram
  wait            = yes
  user            = nobody
  server          = /usr/sbin/in.tftpd
  server_args     = /home/tsrapnik/tftp_root #todo: should be folder of choise.
  disable         = no
}
```

```
sudo service xinetd restart
```

#### debugging
todo:
we start by installing some tools required by openocd. it is quite possible along the process some steps will fail and require installation of some extra tools. i for example had to add `automake` and `pkg-config` for the `./bootstrap` step to succeed and `libusb-1.0-0-dev` because the st link v2.0 strictly requires a 1.x version of libusb. if you encounter such issues, just google the error, install the required extra tools and rerun the last step until it succeeds.

```
sudo apt install autoconf libtool libusb-dev automake pkg-config libusb-1.0-0-dev
```
todo: remark everything only works for linux.
next we will build our openocd executable from source. this is recommended because depending on which target and dongle you use some configuration might differ. choose any directory you like and open it in a terminal. we first clone the source code from a git repository.

```
cd some_directory
git clone --recursive git://git.code.sf.net/p/openocd/code openocd
```

the source code is now in a folder called openocd in you chosen directory. cd into it and just run each command succesively as shown below. if a command fails, fix it as described at the beginning of this chapter.

todo: describe pins, dongle, openocd, gdb.
the only command where you might need to do something different is the `./configure` command. depending on which dongle you use you may have to provide different options. i for example use the C232HM DDHSL-0 which uses MPSSE, 
 --enable-ftdi           Enable building support for the MPSSE mode of FTDI
                          based devices (default is auto)                  
so i have to add the `--enable-ftdi` option for the dongle to be usable. use `./configure --help` to show all available options, here you can find the option for your dongle. at the end of the `./configure` step you should get a summary of all programmers that are available.

-j 8 amount of threads should match cores.

```
./bootstrap
./configure --help
./configure --enable-ftdi
make -j 8
sudo make install
```

```
OpenOCD configuration summary
--------------------------------------------------
MPSSE mode of FTDI based devices        yes
ST-Link Programmer                      yes (auto)
TI ICDI JTAG Programmer                 yes (auto)
Keil ULINK JTAG Programmer              yes (auto)
Altera USB-Blaster II Compatible        yes (auto)
Bitbang mode of FT232R based devices    yes (auto)
Versaloon-Link JTAG Programmer          yes (auto)
TI XDS110 Debug Probe                   yes (auto)
OSBDM (JTAG only) Programmer            yes (auto)
eStick/opendous JTAG Programmer         yes (auto)
Andes JTAG Programmer                   yes (auto)
USBProg JTAG Programmer                 yes (auto)
Raisonance RLink JTAG Programmer        yes (auto)
Olimex ARM-JTAG-EW Programmer           yes (auto)
CMSIS-DAP Compliant Debugger            no
Cypress KitProg Programmer              no
Altera USB-Blaster Compatible           no
ASIX Presto Adapter                     no
OpenJTAG Adapter                        no
SEGGER J-Link Programmer                yes (auto)
```

# sd card becomes read only
sudo mount --options remount,rw /dev/cardname
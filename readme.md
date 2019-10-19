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
this repository was originally built on a ubuntu machine with visual studio code, but it should run fine with linux flavours and editors. to build the project [make](https://www.gnu.org/software/make/), a bash terminal and an arm gcc compiler are needed. make and bash are preinstalled on most linux machines, on windows you'll have to google for an alternative. the project is compiled with [gcc-linaro-7.1.1-2017.08-x86_64_aarch64-elf](https://releases.linaro.org/components/toolchain/binaries/7.1-2017.08/aarch64-elf/). it is advised to use the same compiler (free downloadable), otherwise the binary might not work.

to compile the project the build.sh script should be modified, according to the instructions inside it. this mostly means defining the paths to the compiler and other recourses and choosing between a few options.

the most important option is the boot mode. you can boot from an sd card (fast boot time, ideal for running a finished program) or over the network (boots slower, but is more convenient while programming).

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
screen /dev/...
```

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
  server_args     = /home/choose_location
  disable         = no
}
```

```
sudo service xinetd restart
```
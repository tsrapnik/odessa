# odessa

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

to compile the project the build.sh script should be modified, according to the instructions inside it. this mostly means defining the paths to the compiler and other recourses. if this is done the project can be build by executing the build.sh script (maybe with some commandline option) inside the bash terminal.

```
cd /path_to_project_folder/odessa
chmod +x build.sh
./build.sh some_commandline_option
```

the chmod line needs to be executed only the first time, to make the script executable. when using visual studio code the script can be run with the shortcut ctrl+shift+b.

//todo: explain different command line options. retest and expand network boot, uart, etc.

### network booting

#### enable uart

```
sudo rpi-update
$ strings bootcode.bin | grep BOOT_UART
BOOT_UART=0
$ sed -i -e "s/BOOT_UART=0/BOOT_UART=1/" bootcode.bin
$ strings bootcode.bin | grep BOOT_UART
BOOT_UART=1
```

#### activate network boot mode

get raspbian stretch lite

```
vcgencmd otp_dump | grep 17
1020000a on 3, 3020000a on 3+
boot/config.txt
program_usb_boot_mode=1
```
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
todo: add all references to other projects and documents.
Leon de Boer
rsta2
https://github.com/raspberrypi/firmware/wiki/
todo: explain compile instructions and required toolchain.

draft network boot instructions:
https://metebalci.com/blog/bare-metal-rpi3-network-boot/

get raspbian stertch lite
>vcgencmd otp_dump | grep 17
1020000a on 3, 3020000a on 3+
boot/config.txt
program_usb_boot_mode=1

uart:
>sudo rpi-update
$ strings bootcode.bin | grep BOOT_UART
BOOT_UART=0
$ sed -i -e "s/BOOT_UART=0/BOOT_UART=1/" bootcode.bin
$ strings bootcode.bin | grep BOOT_UART
BOOT_UART=1


on linux (if virtualbox then bridged adapter promiscuous)
sudo tcpdump port bootpc (-v)

sudo apt install isc-dhcp-server
/etc/dhcp/dhcpd.conf

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

sudo service isc-dhcp-server restart

sudo apt install tftpd
/etc/xinetd.d

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
sudo service xinetd restart



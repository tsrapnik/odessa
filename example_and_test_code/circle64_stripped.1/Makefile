all: kernel8.img

kernel8.img: circle.ld startup.o main.o
	$(LD) -o kernel8.elf -Map kernel8.map -T circle.ld startup.o main.o
	$(PREFIX)objdump -d kernel8.elf | $(PREFIX)c++filt > kernel8.lst
	$(PREFIX)objcopy kernel8.elf -O binary kernel8.img
	wc -c kernel8.img

PREFIX	?= aarch64-linux-gnu-

CC	= $(PREFIX)gcc
CPP	= $(PREFIX)g++
AS	= $(CC)
LD	= $(PREFIX)ld
AR	= $(PREFIX)ar

ARCH	?= -march=armv8-a -mtune=cortex-a53 -mlittle-endian -mcmodel=small

AFLAGS	+= $(ARCH)
CFLAGS	+= $(ARCH) -Wall -fno-builtin -nostdinc -nostdlib -O2
CPPFLAGS+= $(CFLAGS) -fno-exceptions -fno-rtti -std=c++0x

%.o: %.S
	$(AS) $(AFLAGS) -c -o $@ $<

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -f *.o *.a *.elf *.lst *.img *.cir *.map *~ $(EXTRACLEAN)


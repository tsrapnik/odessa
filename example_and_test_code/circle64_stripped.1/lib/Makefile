OBJS	= exceptionstub.o sysinit.o

all: startup.o libcircle.a

startup.o: startup.S

libcircle.a: $(OBJS)
	rm -f libcircle.a
	$(AR) cr libcircle.a $(OBJS)

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


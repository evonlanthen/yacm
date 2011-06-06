################################################################################
# Makefile for yacm 
################################################################################

ROOTFS		= /carme/rootfs

# Build settings
CC		= arm-linux-gcc
CFLAGS		= -g -Wall -std=c99 -I$(ROOTFS)/usr/include -I$(ROOTFS)/usr/include/microwin
LDFLAGS 	= -lnano-X -lvncserver -lm -lpng -lfreetype -ljpeg -lz -lSDL -lSDL_mixer -ldirectfb -ldirect -lfusion -lmad -L$(ROOTFS)/usr/lib

# Installation variables
EXEC_NAME	= yacm

# Make rules
all: yacm

yacm: 	${OBJS}
	$(CC) $(CFLAGS) -o $(EXEC_NAME) *.c $(LDFLAGS)

clean:
	$(RM) *.o $(EXEC_NAME)

install:
	cp $(EXEC_NAME) $(ROOTFS)/usr/local/bin
	mkdir -p $(ROOTFS)/usr/local/share/yacm
	cp ../resources/*.mp3 $(ROOTFS)/usr/local/share/yacm/


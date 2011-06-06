################################################################################
# Makefile for yacm 
################################################################################

# Directory of root filesystem
ROOTFS		= /carme/rootfs

# Build settings
CC		= arm-linux-gcc
CFLAGS		= -Wall -std=c99 -I$(ROOTFS)/usr/include -I$(ROOTFS)/usr/include/microwin
LDFLAGS 	= -lnano-X -lvncserver -lm -lpng -lfreetype -ljpeg -lz -lSDL -lSDL_mixer -ldirectfb -ldirect -lfusion -lmad -L$(ROOTFS)/usr/lib

# Installation variables
EXEC_NAME	= yacm

# Make rules
all: orchid carme

orchid:
	$(CC) $(CFLAGS) -o $(EXEC_NAME)_orchid src/*.c $(LDFLAGS)

carme:
	$(CC) -DCARME $(CFLAGS) -o $(EXEC_NAME)_carme src/*.c $(LDFLAGS)

clean:
	$(RM) *.o $(EXEC_NAME)_* $(EXEC_NAME)

orchid-install: resource-install
	cp $(EXEC_NAME)_orchid $(ROOTFS)/usr/local/bin

carme-install: resource-install
	cp $(EXEC_NAME)_carme $(ROOTFS)/usr/local/bin
	
resource-install:
	mkdir -p $(ROOTFS)/usr/local/share/yacm
	cp resources/*.mp3 $(ROOTFS)/usr/local/share/yacm/

install:
	@echo "Please use 'make orchid-install' or 'make carme-install'"


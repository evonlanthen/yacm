/**
 * Functions for memory mapping
 *
 * @file    mmap.c
 * @version 0.1
 * @author  Elmar Vonlanthen (vonle1@bfh.ch)
 * @date    May 27, 2011
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "defines.h"
#include "types.h"
#include "mmap.h"

#ifdef CARME
 #include "carme.h"
#elif defined(ORCHID)
 #include "orchid.h"
#else
 #error "no board defined!"
#endif

#define MEM_DEV "/dev/mem"

// needs to be global for gpio.c:
char *mmap_base = NULL;

static int fd_mem;
static off_t io_base = IO_BASE;
static int isMmapSetUp = FALSE;

int setUpMmap(void)
{
	if (isMmapSetUp) {
		return FALSE;
	}
	/* Try to open the mem device special file */
	if ((fd_mem = open(MEM_DEV, O_RDWR | O_SYNC)) < 1) {
		perror("open(\"/dev/mem\")");
		return FALSE;
	}

	/* Map memory region for memory controller registers */
	mmap_base = mmap(NULL,				/* Start address */
				MAP_SIZE,				/* The IO space */
				PROT_READ | PROT_WRITE, /* Allow read and write */
				MAP_SHARED,				/* Share this mapping with all processes */
				fd_mem,					/* Device is /dev/mem */
				io_base);				/* IO memory region base */

	if (mmap_base == (void*) -1) {
		perror("mmap()");
		return(FALSE);
	}

	#ifdef ORCHID
		GPIO_init();
	#endif

	isMmapSetUp = TRUE;
	return TRUE;
}

int tearDownMmap(void)
{
	if (!isMmapSetUp) {
		return FALSE;
	}
	munmap(mmap_base, MAP_SIZE);
	close(fd_mem);
	isMmapSetUp = FALSE;
	return TRUE;
}

int getMmapSetUpState(void)
{
	return isMmapSetUp;
}
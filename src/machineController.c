/*
 * machineController.c
 *
 *  Created on: May 20, 2011
 *      Author: elmar
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <time.h>
#include <fcntl.h>

#include "defines.h"
#include "types.h"
#include "machineController.h"

#ifdef CARME
 #include "carme.h"
#elif defined(ORCHID)
 #include "orchid.h"
#else
 #error "no board defined!"
#endif

#define MEM_DEV "/dev/mem"

char *mmap_base = NULL;
int fd_mem;
static off_t io_base = IO_BASE;

int initMachineController(void)
{
	/* Try to open the mem device special file */
	if ((fd_mem = open(MEM_DEV, O_RDWR | O_SYNC)) < 1) {
		perror("open(\"/dev/mem\")");
		return(FALSE);
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

	return TRUE;
}

int cleanUpMachineController(void) {
	munmap(mmap_base, MAP_SIZE);
	close(fd_mem);
	return TRUE;
}

int startMachine(void)
{
	int ret = TRUE;
	return ret;
}

int stopMachine(void)
{
	int ret = TRUE;
	return ret;
}

int machineRunning(void)
{
	int ret = TRUE;
	return ret;
}

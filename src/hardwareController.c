/**
 * @brief   Initialize memory mapping and gpio's
 * @file    hardwareController.c
 * @version 1.0
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
#include "hardwareController.h"

#ifdef CARME
 #include "carme.h"
#elif defined(ORCHID)
 #include "orchid.h"
#else
 #error "no board defined!"
#endif

/**
 * Defines
 */

#define MEM_DEV "/dev/mem"

/**
 * Global variables
 */

// needs to be global for carme.c or orchid.c
void *mmap_base = NULL;

/**
 * Local module specific variables
 */
static int fd_mem;
static off_t io_base = IO_BASE;
static int isHardwareSetUp = FALSE;

/**
 * @copydoc setUpHardwareController
 */
int setUpHardwareController(void)
{
	// Check if hardware controller is already setup
	if (isHardwareSetUp) {
		return FALSE;
	}
	// Try to open the mem device special file
	if ((fd_mem = open(MEM_DEV, O_RDWR | O_SYNC)) < 1) {
		perror("open(\"/dev/mem\")");
		return FALSE;
	}

	// Map memory region for memory controller registers:
	mmap_base = mmap(NULL,				// Start address
				MAP_SIZE,				// The IO space
				PROT_READ | PROT_WRITE, // Allow read and write
				MAP_SHARED,				// Share this mapping with all processes
				fd_mem,					// Device is /dev/mem
				io_base);				// IO memory region base

	// check if memory is successfully mapped:
	if (mmap_base == (void*) -1) {
		perror("mmap()");
		return(FALSE);
	}

	// Initialize GPIO for CARME (only for buttons) or ORCHID board
#ifdef CARME
	setUpCarmeGPIO();
#elif defined(ORCHID)
	GPIO_init();
#endif

	isHardwareSetUp = TRUE;
	return TRUE;
}

/**
 * @copydoc tearDownHardwareController
 */
int tearDownHardwareController(void)
{
	// check if hardware was initialized
	if (!isHardwareSetUp) {
		return FALSE;
	}
	// unmap memory and free filedescriptor:
	munmap(mmap_base, MAP_SIZE);
	close(fd_mem);
#ifdef CARME
	// tear down GPIO button configuration if CARME board:
	tearDownCarmeGPIO();
#endif
	isHardwareSetUp = FALSE;
	return TRUE;
}

/**
 * @copydoc getHardwareSetUpState
 */
int getHardwareSetUpState(void)
{
	return isHardwareSetUp;
}

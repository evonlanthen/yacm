/**
 * @brief   Initialize memory mapping and gpio's
 * @file    hardwareController.h
 * @version 1.0
 * @author  Elmar Vonlanthen (vonle1@bfh.ch)
 * @date    May 27, 2011
 */

#ifndef HARDWARECONTROLLER_H_
#define HARDWARECONTROLLER_H_

/**
 * Global pointer to memory mapping base address
 */
extern void *mmap_base;

/**
 * Initializes the hardware controller
 *
 * @return Returns TRUE if initialization was successful
 */
extern int setUpHardwareController(void);

/**
 * Tears down hardware controller
 *
 * @return Returns TRUE if tearing down was successful
 */
extern int tearDownHardwareController(void);

/**
 * Get status of hardware setup
 *
 * @return Returns TRUE if hardware was set up
 */
extern int getHardwareSetUpState(void);

#endif /* HARDWARECONTROLLER_H_ */

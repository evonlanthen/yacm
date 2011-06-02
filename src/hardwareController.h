/**
 * Initialize memory mapping and gpio's
 *
 * @file    hardwareController.h
 * @version 0.1
 * @author  Elmar Vonlanthen (vonle1@bfh.ch)
 * @date    May 27, 2011
 */

#ifndef HARDWARECONTROLLER_H_
#define HARDWARECONTROLLER_H_

extern void *mmap_base;

extern int setUpHardwareController(void);

extern int tearDownHardwareController(void);

extern int getHardwareSetUpState(void);

#endif /* HARDWARECONTROLLER_H_ */

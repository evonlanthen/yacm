/**
 * Definitions for board CARME
 *
 * @file    carme.h
 * @version 0.1
 * @author  Elmar Vonlanthen (vonle1@bfh.ch)
 * @date    May 20, 2011
 */

#ifndef CARME_H_
#define CARME_H_

/*
 *******************************************************************************
 * Definitions
 *******************************************************************************
 */

#define MAP_SIZE		(6*4096)	/* Six pages */
#define IO_BASE			(0xc000000)	/* IO base in physical memory */
#define LED_OFFSET		(0x3000)	/* Offset of led register in IO space */
#define SWITCH_OFFSET	(0x3200)	/* Offset of switch register in IO space */

/*
 *******************************************************************************
 * Public Function Prototypes
 *******************************************************************************
 */

extern int setUpCarmeGPIO();
extern int tearDownCarmeGPIO();
extern int readGPIOButton(int id);

#endif /* CARME_H_ */

/**
 ***************************************************************************
 * \brief   Header for the CARME Board
 * \file    carme.h
 * \version 1.0
 * \date    28.09.2008
 * \author  Martin Aebersold
 *
 * \remark  Last Modifications:
 * \remark  V1.0, AOM1, 28.08.07   Initial release
 * \remark  V1.1, 02.06.2011       Add GPIO functions
 * 
 ****************************************************************************
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

/**
 * Initializing GPIOs
 *
 * Exporting button GPIOs and setting direction to 'in'.
 *
 * @return Returns TRUE if initializing was successful
 */
extern int setUpCarmeGPIO();

/**
 * Tearing down GPIOs
 *
 * Unexporting button GPIOs
 *
 * @return Returns TRUE if tearing down was successful
 */
extern int tearDownCarmeGPIO();

/**
 * Read button state over GPIO
 *
 * @param id Button identifier
 * @return Returns state of button (on or off)
 */
extern int readGPIOButton(int id);

#endif /* CARME_H_ */

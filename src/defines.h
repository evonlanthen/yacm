/**
 * Preprocessor definitions
 *
 * @file    defines.h
 * @version 0.1
 * @author  Elmar Vonlanthen (vonle1@bfh.ch)
 * @date    May 20, 2011
 */

#ifndef DEFINES_H_
#define DEFINES_H_

/**
 * Define the BOARD to use (CARME or ORCHID)
 */
//#define CARME
#ifndef CARME
	#define ORCHID
#endif


/**
 * Define symbol for the boolean value 'true'.
 */
#define TRUE 1
/**
 * Define symbol for the boolean value 'false'.
 */
#define FALSE 0

/**
 * Define NULL pointer
 */
#ifndef NULL
 #define NULL ((void*)0)
#endif

/**
 * Define end of file value
 */
#define EOF (-1)

#endif /* DEFINES_H_ */

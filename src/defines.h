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
#define	ORCHID
#undef CARME

/**
 * Define symbols for the boolean values true and false
 */
#define TRUE 1
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

/**
 * @file   ledController.h
 * @author Elmar Vonlanthen (vonle1@bfh.ch)
 * @date   May 20, 2011
 * @brief  Controls the LEDs.
 *
 * Initializes and controls the LEDs.
 */

#ifndef LEDCONTROLLER_H_
#define LEDCONTROLLER_H_

/**
 * Initialize LEDs
 * @see initLeds()
 */
extern int initLeds(void);
/**
 * Set one single LED
 * @param num Select the LED number.
 * @param enable If set to TRUE, the LED will be enabled, else disabled.
 * @return success value
 */
extern int setLed(int num, int enable);

#endif /* LEDCONTROLLER_H_ */

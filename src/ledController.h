/**
 * LED controlling
 *
 * Initialize and control the LEDs.
 *
 * @file    ledController.h
 * @version 0.1
 * @author  Elmar Vonlanthen (vonle1@bfh.ch)
 * @date    May 20, 2011
 */

#ifndef LEDCONTROLLER_H_
#define LEDCONTROLLER_H_

#ifdef CARME
 #define	LED_1			(1 << 0)
 #define	LED_2			(1 << 1)
 #define	LED_3			(1 << 2)
 #define	LED_4			(1 << 3)
 #define	LED_5			(1 << 4)
 #define	LED_6			(1 << 5)
 #define	LED_7			(1 << 6)
 #define	LED_8			(1 << 7)
 #define    NUM_OF_LEDS     8
#elif defined(ORCHID)
 #define	LED_1			(1 << 0)
 #define	LED_2			(1 << 1)
 #define	LED_3			(1 << 2)
 #define	LED_4			(1 << 3)
 #define	LED_5			(1 << 4)
 #define	LED_6			(1 << 5)
 #define	LED_7			(1 << 6)
 #define	LED_8			(1 << 7)
 #define    NUM_OF_LEDS     8
#endif
/* Get LED ID: */
#define		LED_ID(x) (1 << ((x)-1))


/**
 * Predefined LED states
 */
enum LedState {
  led_off = 0,
  led_on,
  led_blinking
};

/**
 * Initialize LED controller
 *
 * @return Returns TRUE if initializing was successful
 */
extern int initLedController(void);

/**
 * Clean up LED controller
 *
 * @return Returns TRUE if cleaning up was successful
 */
extern int cleanUpLedController(void);

/**
 * Update LED state
 *
 * Update LED according to the overall (on/off) or blinking state.
 *
 * @param id LED identifier
 * @param state LED state (on, off or blinking)
 * @return Returns TRUE if LED updating was successful
 */
extern int updateLed(int id, enum LedState state);

/**
 * Set LED blinking interval
 *
 * Set the LED blinking interval values in milliseconds
 *
 * @param id LED indentifier
 * @param intervalOn Duration while LED is on
 * @param intervalOff Duration while LED is off
 */
extern int setBlinkingFreq(int id, int intervalOn, int intervalOff);

#endif /* LEDCONTROLLER_H_ */

/**
 * @brief   Initialize and read the sensors
 * @file    sensorController.h
 * @version 1.0
 * @author  Elmar Vonlanthen (vonle1@bfh.ch)
 * @date    May 20, 2011
 */

#ifndef SENSORCONTROLLER_H_
#define SENSORCONTROLLER_H_

/**
 * Sensor definitions
 */
#ifdef CARME
  #define SENSOR_1		(1 << 6)
  #define SENSOR_2		(1 << 7)
#elif defined(ORCHID)
  #define SENSOR_1		(1 << 2)
  #define SENSOR_2		(1 << 3)
#endif

/**
 * Predefined sensor states
 */
enum SensorState {
  sensor_normal = 0, /**< sensor_normal  */
  sensor_alert,      /**< sensor_alert   */
  sensor_unknown     /**< sensor_unknown */
};

/**
 * Initializes the sensor controller
 *
 * @return Returns TRUE if initialization was successful
 */
extern int setUpSensorController(void);

/**
 * Tears down sensor controller
 *
 * @return Returns TRUE if tearing down was successful
 */
extern int tearDownSensorController(void);

/**
 * Checks a sensor of its state
 *
 * @param id Id of sensor to check
 * @return State of sensor (normal, alert or unknown)
 */
extern enum SensorState getSensorState(int id);

#endif /* SENSORCONTROLLER_H_ */

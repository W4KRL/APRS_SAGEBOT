/**
 * @file taskControl.h
 * @author Karl Berger
 * @date 2025-06-04
 * @brief Declarations for task scheduling and control using TickTwo timers.
 *
 * This header defines external TickTwo timer objects for various periodic tasks,
 * such as updating clocks, fetching weather data, and posting updates to APRS and ThingSpeak.
 * It also declares functions to start and update these scheduled tasks.
 *
 * External TickTwo timers:
 * - tmrSecondTick: Timer for second tick clock updates.
 * - tmrGetWXcurrent: Timer for current weather updates.
 * - tmrGetWXforecast: Timer for forecasted weather updates.
 * - tmrPostWXtoAPRS: Timer for posting weather data to APRS.
 * - tmrPostWXtoThingspeak: Timer for posting weather data to ThingSpeak.
 * - tmrUpdateFrame: Timer for sequential frame updates.
 *
 * Functions:
 * - startTasks(): Start the scheduled tasks.
 * - updateTasks(): Update the scheduled tasks.
 */
#ifndef TASK_CONTROL_H
#define TASK_CONTROL_H

#include <Arduino.h> // for String
#include <TickTwo.h> // v4.4.0 Stefan Staub

extern TickTwo tmrSecondTick;		  ///< timer for second tick clock updates
extern TickTwo tmrGetWXcurrent;		  ///< timer for current weather updates
extern TickTwo tmrGetWXforecast;	  ///< timer for forecasted weather updates
extern TickTwo tmrPostWXtoAPRS;		  ///< timer for posting weather data to APRS
extern TickTwo tmrPostWXtoThingspeak; ///< timer for posting weather data to ThingSpeak
extern TickTwo tmrUpdateFrame;		  ///< timer for sequential frames

void startTasks();	///< start the scheduled tasks
void updateTasks(); ///< update the scheduled tasks

#endif // TASK_CONTROL_H
	   // End of file
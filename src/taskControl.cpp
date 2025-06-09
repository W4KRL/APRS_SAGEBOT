/**
 * @file taskControl.cpp
 * @author KarlB
 * @date   2024-06-09
 * @brief Implements scheduled task management using TickTwo timers for weather data retrieval, posting, and display updates.
 *
 * This file sets up and manages periodic tasks for:
 * - Fetching current and forecasted weather data.
 * - Posting weather data to APRS and ThingSpeak services.
 * - Updating sequential display frames and clock ticks.
 *
 * Timers are instantiated using the TickTwo library and started in the `startTasks()` function, which should be called in the Arduino `setup()`.
 * The `updateTasks()` function should be called in the Arduino `loop()` to ensure timers are serviced and scheduled tasks are executed.
 *
 * Dependencies:
 * - Arduino.h: Core Arduino functions.
 * - TickTwo.h: Timer library for scheduling tasks.
 * - aprsService.h: APRS posting functions.
 * - credentials.h: Interval definitions and credentials.
 * - sequentialFrames.h: Display frame management.
 * - thingSpeakService.h: ThingSpeak posting functions.
 * - weatherService.h: Weather data retrieval.
 */
#include "taskControl.h" // task control functions

#include <Arduino.h>		   // Arduino functions
#include <TickTwo.h>		   // v4.4.0 Stefan Staub https://github.com/sstaub/TickTwo
#include "aprsService.h"	   // APRS functions
#include "credentials.h"	   // for WX_CURRENT_INTERVAL, WX_FORECAST_INTERVAL, etc.
#include "sequentialFrames.h"  // sequential weather and almanac frames
#include "thingSpeakService.h" // ThingSpeak posting
#include "weatherService.h"	   // weather data from Weather Underground API

//! Instantiate the scheduled tasks
TickTwo tmrGetWXcurrent(getWXcurrent, WX_CURRENT_INTERVAL * 60 * 1000, 0, MILLIS);
TickTwo tmrGetWXforecast(getWXforecast, WX_FORECAST_INTERVAL * 60 * 1000, 0, MILLIS);
TickTwo tmrPostWXtoAPRS(postWXtoAPRS, WX_APRS_INTERVAL * 60 * 1000, 0, MILLIS);
TickTwo tmrPostWXtoThingspeak(postWXtoThingspeak, TS_POST_INTERVAL * 60 * 1000, 0, MILLIS);
TickTwo tmrUpdateFrame(updateSequentialFrames, SCREEN_DURATION * 1000, 0, MILLIS);
TickTwo tmrSecondTick(updateClock, 1000, 0, MILLIS);

//! Start the TickTwo timers in setup()
void startTasks()
{
	tmrGetWXcurrent.start();	   // timer for current weather
	tmrGetWXforecast.start();	   // timer for forecasted weather
	tmrPostWXtoThingspeak.start(); // timer for posting to ThingSpeak
	tmrPostWXtoAPRS.start();	   // timer for posting weather to APRS
	tmrUpdateFrame.start();		   // timer for sequential frames
								   // tmrSecondTick is started in the clock frame
} // startTasks()

//! Update the TickTwo timers in loop()
void updateTasks()
{
	tmrGetWXcurrent.update();		// get current weather
	tmrPostWXtoThingspeak.update(); // post selected current weather to ThingSpeak
	tmrPostWXtoAPRS.update();		// post selected weather data to APRS
	tmrGetWXforecast.update();		// get forecasted weather for display
	tmrUpdateFrame.update();		// update sequential frames
	tmrSecondTick.update();			// update seconds for clock & frame displays
} // updateTasks()
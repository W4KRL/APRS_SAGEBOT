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

#include <Arduino.h>	 // Arduino functions
#include <TickTwo.h>	 // v4.4.0 Stefan Staub https://github.com/sstaub/TickTwo
#include "aprsService.h" // APRS functions

//! Instantiate the scheduled tasks
TickTwo tmrAPRSticker(pollAPRS, 5000, MILLIS); // APRS bulletin ticker

//! Start the TickTwo timers in setup()
void startTasks()
{
	tmrAPRSticker.start(); // start APRS ticker
} // startTasks()

//! Update the TickTwo timers in loop()
void updateTasks()
{
	tmrAPRSticker.update(); // update APRS ticker
} // updateTasks()
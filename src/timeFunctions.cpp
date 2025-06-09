/**
 * @file timeFunctions.cpp
 * @author Karl W. Berger
 * @date 2025-06-09
 * @brief Timezone management implementation.
 *
 * This file provides functions for managing and setting the timezone
 * using the ezTime library. It includes functionality to set the timezone
 * based on a cached value or a user-defined value, and to convert
 * 24-hour format hours to 12-hour format.
 *
 * Functions:
 * - setTimeZone(): Sets the timezone using the ezTime library and user credentials.
 * - to12HourFormat(int hour): Converts a 24-hour format hour to 12-hour format.
 */

#include "timeFunctions.h"

#include <Arduino.h>	 // Arduino functions
#include <ezTime.h>		 // ezTime library for timezone handling
#include "aprsService.h" // for APRSsendBulletin
#include "credentials.h" // for MY_TIMEZONE

Timezone myTZ;

/**
 * @brief Sets the local timezone configuration.
 *
 * This function synchronizes the system time and ensures that the timezone is set correctly.
 * If the timezone has not been cached in EEPROM or if the user requests a new timezone,
 * it updates the timezone using the specified location. Finally, it sets the local timezone as default.
 *
 * Dependencies:
 * - Requires `myTZ` object with methods: setCache(), getOlson(), setLocation(), setDefault().
 * - Uses `MY_TIMEZONE` constant for the desired timezone.
 * - Calls `waitForSync()` to ensure time synchronization before setting the timezone.
 */
void setTimeZone()
{
	waitForSync();
	if (!myTZ.setCache(0) || myTZ.getOlson() != MY_TIMEZONE)
	{
		myTZ.setLocation(MY_TIMEZONE);
	}
	myTZ.setDefault(); // set local timezone
}

/**
 * @brief Converts a 24-hour format hour to a 12-hour format hour.
 *
 * @param hour The hour in 24-hour format (0-23).
 * @return int The hour in 12-hour format (1-12).
 */
int to12HourFormat(int hour)
{
	return (hour == 0) ? 12 : (hour > 12) ? (hour - 12)
										  : hour;
}
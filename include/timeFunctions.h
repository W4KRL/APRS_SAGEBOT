/**
 * @file timeFunctions.h
 * @author Karl W. Berger
 * @date 2025-06-05
 * @brief Global variables and declarations for timezone management.
 */

#ifndef TIME_FUNCTIONS_H
#define TIME_FUNCTIONS_H

#include <Arduino.h>
#include <ezTime.h> // for Timezone		

#ifdef DEBUG
#undef DEBUG // Prevent conflicts with other libraries
#endif

/**
 * @brief Externally declared Timezone object representing the application's current timezone.
 *
 * This global variable provides access to the configured timezone settings,
 * allowing time conversions and timezone-aware operations throughout the application.
 *
 * @note The actual definition and initialization of `myTZ` must be provided elsewhere in the codebase.
 */
extern Timezone myTZ;

void setTimeZone();			  // Function to set the timezone
int to12HourFormat(int hour); // Convert 24-hour format to 12-hour format

#endif
// End of file

/**
 * @file wug_debug.h
 * @author Karl Berger
 * @date 2025-05-28
 * @brief Debugging functions for serial output.
 * @details
 * This header provides macros for printing debug messages to the serial monitor.
 * The macros are enabled when WUG_DEBUG is defined, allowing conditional
 * compilation of debug output. This helps in troubleshooting and monitoring
 * program execution without affecting release builds.
 *
 * Usage:
 * - Define WUG_DEBUG before including this header to enable debug prints.
 * - Use DEBUG_PRINT(x) and DEBUG_PRINTLN(x) for debug output.
 */

#ifndef DEBUG_H
#define DEBUG_H	

#include <Arduino.h>  // Required for String

#ifdef DEBUG
#undef DEBUG  // Prevent conflicts with other libraries
#endif

//! Debug print macro
#ifdef WUG_DEBUG
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif

#endif
// End of file

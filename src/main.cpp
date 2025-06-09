/**
 * @file main.cpp
 * @author Karl Berger
 * @date 2025-06-05
 * @brief Main program for Weather Underground, APRS-IS and Thingspeak integration
 * @details This program initializes communication with sensors and processes input data
 *          from the Weather Underground API, and formats it for display and posting to
 *          ThingSpeak, and APRS-IS.
 * @todo read day/night indicator and choose json data accordingly
 * @todo add APRS bulletin for sunrise/sunset
 * @todo test WUG API responses for absence. Do not update WX data if response missing
 * @todo add WiFiManager for configuration or SCPI commands
 *
 * @see [GitHub Repository](https://github.com/W4KRL/WUG_APRS_TS)
 */

#define WUG_DEBUG //! uncomment this line for serial debug output

/*
******************************************************
******************** INCLUDES ************************
******************************************************
*/
#include <Arduino.h>           // Arduino functions
#include "analogClock.h"       // analog clock functions
#include "aphorismGenerator.h" // aphorism functions
#include "aprsService.h"       // APRS functions
#include "credentials.h"       // account information
#include "digitalClock.h"      // digital clock display
#include "indoorSensor.h"      // indoor sensor functions
#include "onetimeScreens.h"    // splash screen and information screens
#include "sequentialFrames.h"  // sequential weather, almanac, and clock frames
#include "taskControl.h"       // task control functions
#include "tftDisplay.h"        // TFT display functions
#include "thingSpeakService.h" // ThingSpeak posting
#include "timeFunctions.h"     // timezone object
#include "unitConversions.h"   // unit conversions
#include "weatherService.h"    // weather data from Weather Underground API
#include "wifiConnection.h"    // Wi-Fi connection
#include "wug_debug.h"         // debug print macro

/*
******************************************************
********************* SETUP **************************
******************************************************
*/
void setup()
{
  Serial.begin(115200); // initialize serial monitor
  initSensor();         // initialize indoor sensor
  setupTFTDisplay();    // initialize TFT display
  splashScreen();       // stays on until logon is complete
  logonToRouter();      // connect to WiFi
  getWXcurrent();       // find latitude & longitude for your weather station
  setTimeZone();        // set timezone
  mountFS();            // mount LittleFS and prepare APRS bulletin file
  dataScreen();         // show configuration data
  getWXforecast();      // initialize weather API: needs lat/lon from getWXcurrent
  delay(2000);          // delay to show connection info
  startTasks();         // start the scheduled tasks
} // setup()

/*
******************************************************
********************* LOOP ***************************
******************************************************
*/
void loop()
{
  checkWiFiConnection(); // check Wi-Fi connection status
  events();              // ezTime events including autoconnect to NTP server
  processBulletins();    // process APRS bulletins
  updateTasks();         // update the scheduled tasks
} // loop()

/*
*******************************************************
******************* END OF CODE ***********************
*******************************************************
*/
/**
 * @file main.cpp
 * @author Karl Berger
 * @date 2025-06-05
 * @brief Main program for APRS_SAGEBOT integration
 * @details This program initializes communication with APRS-IS, processes input
 *          messages, and responds with a message.
 *
 * @see [GitHub Repository](https://github.com/W4KRL/APRS_SAGEBOT)
 */

#define WUG_DEBUG //! uncomment this line for serial debug output

/*
******************************************************
******************** INCLUDES ************************
******************************************************
*/
#include <Arduino.h>           // Arduino functions
#include "aphorismGenerator.h" // aphorism functions
#include "aprsService.h"       // APRS functions
#include "credentials.h"       // account information
#include "tftDisplay.h"        // TFT display functions
#include "timeFunctions.h"     // timezone object
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
  setupTFTDisplay();    // initialize TFT display
  logonToRouter();      // connect to WiFi
  logonToAPRS();        // connect to APRS-IS server
  mountFS();            // mount LittleFS and prepare APRS bulletin file
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
  // updateTasks();         // update the scheduled tasks
} // loop()

/*
*******************************************************
******************* END OF CODE ***********************
*******************************************************
*/
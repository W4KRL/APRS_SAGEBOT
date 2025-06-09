/**
 * @file thingSpeakService.cpp
 * @author Karl W. Berger
 * @date 2025-05-28
 * @brief Posts weather observation data to ThingSpeak.
 *
 * Establishes a connection to the ThingSpeak server and sends the current weather
 * data (temperature, humidity, pressure, wind speed, wind direction, solar radiation,
 * precipitation total, and precipitation rate) as fields 1-8. Optionally includes a
 * status message if set. Uses the API key defined in TS_WRITE_KEY and posts data
 * using HTTP POST to the /update endpoint.
 *
 * Dependencies:
 * - Requires WiFi connection to be established.
 * - Uses global weather data from the wx object.
 * - Uses the global unitStatus string for optional status updates.
 *
 * Debug output is printed via DEBUG_PRINT macros.
 */
#include "thingSpeakService.h"

#include <WiFiClient.h>     // ThingSpeak connection
#include "credentials.h"    // Wi-Fi and weather station credentials
#include "weatherService.h" // weather data
#include "wug_debug.h"      // debug print

//! ************** THINGSPEAK ACCOUNT ********************
#define THINGSPEAK_SERVER "api.thingspeak.com" // ThingSpeak Server
String unitStatus = "";                        // ThingSpeak status global

void postWXtoThingspeak()
{
  WiFiClient client;
  // assemble and post the data
  if (client.connect(THINGSPEAK_SERVER, 80))
  {
    DEBUG_PRINT("ThingSpeak Server connected to channel: ");
    DEBUG_PRINTLN(TS_CHANNEL);

    String dataStr = "&field1=" + String(wx.obsTemp);
    dataStr += "&field2=" + String(wx.obsHumidity);
    dataStr += "&field3=" + String(wx.obsPressure);
    dataStr += "&field4=" + String(wx.obsWindSpeed);
    dataStr += "&field5=" + String(wx.obsWindDir);
    dataStr += "&field6=" + String(wx.obsSolarRadiation);
    dataStr += "&field7=" + String(wx.obsPrecipTotal);
    dataStr += "&field8=" + String(wx.obsPrecipRate);
    dataStr += (unitStatus.isEmpty()) ? "" : ("&status=" + unitStatus);

    DEBUG_PRINTLN(dataStr); // show ThingSpeak payload on serial monitor

    // post the data to ThingSpeak
    client.println("POST /update HTTP/1.1");
    client.print("Host: ");
    client.println(THINGSPEAK_SERVER);
    client.println("Connection: close");
    client.print("X-THINGSPEAKAPIKEY: ");
    client.println(TS_WRITE_KEY);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Content-Length: " + String(dataStr.length()));
    client.println("");
    client.print(dataStr);

    DEBUG_PRINTLN("ThingSpeak data sent.");
  }
  client.stop();
} // postToThingSpeak()
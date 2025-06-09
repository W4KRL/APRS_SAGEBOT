/**
 * @file wifiConnection.cpp
 * @author Karl W. Berger
 * @date 2025-06-05
 * @brief Contains the function to connect to the Wi-Fi network.
 * @details The function logonToRouter() connects the ESP8266 to the specified Wi-Fi network.
 * Uses the WiFi library to connect and prints the IP address once connected.
 */

#include "wifiConnection.h"

#include <Arduino.h>	 // Arduino functions
#include <ESP8266WiFi.h> // [manager] v2.0.0 Wi-Fi
#include "credentials.h" // Wi-Fi credentials
#include "wug_debug.h"	 // debug print

void logonToRouter()
{
	pinMode(LED_BUILTIN, OUTPUT);		  // Built-in LED
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // Begin WiFi
	WiFi.mode(WIFI_STA);				  // Explicitly set mode, ESP defaults to STA+AP
	while (WiFi.status() != WL_CONNECTED) // Wait for Wi-Fi to connect
	{
		delay(500);
		digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // Toggle LED
		DEBUG_PRINT('.');
	}
	digitalWrite(LED_BUILTIN, HIGH); // Turn off LED
	DEBUG_PRINT("\nWi-Fi connected. IP address: ");
	DEBUG_PRINTLN(WiFi.localIP()); // Send the IP address of the ESP8266 to the computer
} // logonToRouter()

void checkWiFiConnection()
{
	if (WiFi.status() != WL_CONNECTED) // Check if Wi-Fi is connected
	{
		logonToRouter(); // Reconnect to Wi-Fi
	}
} // checkWiFiConnection()	
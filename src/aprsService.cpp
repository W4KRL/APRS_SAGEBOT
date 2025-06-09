/**
 * @file aprsService.cpp
 * @brief Contains implementation functions for posting weather data and bulletins to APRS-IS.
 * @author Karl Berger
 * @date 2025-06-09
 */

#include "aprsService.h"

#include <Arduino.h> // Arduino functions
#include "aphorismGenerator.h" // aphorism generator for bulletins
#include "credentials.h"	   // APRS, Wi-Fi and weather station credentials
#include "timeFunctions.h"	   // time functions
#include "unitConversions.h"   // unit conversion functions
#include "wug_debug.h" // debug print

#include <WiFiClient.h> // APRS connection
WiFiClient client;

//! ***************** APRS *******************
//            !!! DO NOT CHANGE !!!
// for list of tier 2 servers: http://www.aprs2.net/
// North America: noam.aprs2.net
// South America: soam.aprs2.net
// Europe: euro.aprs2.net
// Asia: asia.aprs2.net
// Africa: africa.aprs2.net
// Oceania: apan.aprs2.net
const char *APRS_SERVER = "noam.aprs2.net";					  // recommended for North America
const char *APRS_DEVICE_NAME = "https://w4krl.com/iot-kits/"; // link to my website
#define APRS_SOFTWARE_NAME "D1S-VEVOR"						  // unit ID
#define APRS_SOFTWARE_VERS FW_VERSION						  // FW version
#define APRS_PORT 14580										  // do not change port
#define APRS_TIMEOUT 2000L									  // milliseconds

//! ************ APRS Bulletin globals ***************
// int *lineArray;				 // holds shuffled index to aphorisms
int lineCount;				 // number of aphorisms in file
bool amBulletinSent = false; // APRS morning bulletin
bool pmBulletinSent = false; // APRS evening bulletin
int lineIndex = 1;			 // APRS bulletin index

/*
*******************************************************
**************** Logon to APRS-IS *****************
*******************************************************
*/

void logonToAPRS()
{
	// Attempt connection to APRS-IS server
	if (client.connect(APRS_SERVER, APRS_PORT))
	{
		DEBUG_PRINTLN(F("APRS connected"));
	}
	else
	{
		DEBUG_PRINTLN(F("APRS connection failed."));
		return;
	}

	String rcvLine = client.readStringUntil('\n');
	DEBUG_PRINTLN("Rcvd: " + rcvLine);

	if (rcvLine.indexOf("full") > 0)
	{
		DEBUG_PRINTLN(F("APRS port full. Retrying."));
		client.stop();
		delay(500);

		if (client.connect(APRS_SERVER, APRS_PORT))
		{
			DEBUG_PRINTLN(F("APRS reconnected successfully."));
		}
		else
		{
			DEBUG_PRINTLN(F("APRS reconnection failed."));
			return;
		}
	}

	// Send APRS-IS logon info
	String dataString = "user " + CALLSIGN + " pass " + APRS_PASSCODE;
	dataString += " vers IoT-Kits " + APRS_SOFTWARE_VERS;
	client.println(dataString);
	DEBUG_PRINTLN("APRS logon: " + dataString);

	boolean verified = false;
	unsigned long timeBegin = millis();

	while (!verified && (millis() - timeBegin < APRS_TIMEOUT))
	{
		if (client.available())
		{
			String rcvLine = client.readStringUntil('\n');
			DEBUG_PRINTLN("Rcvd: " + rcvLine);

			if (rcvLine.indexOf("verified") != -1 && rcvLine.indexOf("unverified") == -1)
			{
				verified = true;
			}
		}
		yield();
	}

	if (!verified)
	{
		DEBUG_PRINTLN("APRS user unverified.");
		return;
	}
}

void checkAPRSConnection()
{
	if (!client.connected())
	{
		DEBUG_PRINTLN(F("APRS connection lost. Reconnecting..."));
		logonToAPRS();
	}
}

/*
*******************************************************
************** Format Bulletin for APRS-IS ************
*******************************************************
*/
String APRSformatBulletin(String message, String ID)
{
	// format bulletin or announcement
	/* APRS101.pdf pg 83
	 * Bulletin ID is a single digit from 0 to 9
	 * Announcement ID is a single upper-case letter from A to Z
	 * Message may not contain | or ~ or `
	 *  ____________________________
	 *  |:|BLN|ID|-----|:| Message |
	 *  |1| 3 | 1|  5  |1| 0 to 67 |
	 *  |_|___|__|_____|_|_________|
	 */
	String str = CALLSIGN + ">APRS,TCPIP*:" + ":BLN" + ID + "     :" + message;
	DEBUG_PRINTLN("APRS Bulletin: " + str);
	return str;
} // APRSformatBulletin()

/*
*******************************************************
****************** APRS padder ************************
*******************************************************
*/
String APRSpadder(float value, int width)
{
	// pads APRS rounded data element with leading 0s to the specified width
	int val = round(value);
	char format[6]; // Stores the string format specifier (e.g., "%04d")
	snprintf(format, sizeof(format), "%%0%dd", width);
	char paddedValue[width + 1]; // Buffer to store the formatted string
	snprintf(paddedValue, sizeof(paddedValue), format, val);
	return paddedValue;
} // APRSpadder()

/*
*******************************************************
*********** Format callsign for APRS telemetry ********
*******************************************************
*/
String APRSpadCall(String callSign)
{
	// 12/20/2024
	// pad to 9 characters including the SSID pg 12, 127
	char paddedCall[10]; // 9 chars + null terminator
	// print at most 9 characters
	snprintf(paddedCall, sizeof(paddedCall), "%-9.9s", callSign.c_str());
	return String(paddedCall);
} // APRSpadCall()

/*
*******************************************************
*************** Format location for APRS **************
*******************************************************
*/
String APRSlocation(float lat, float lon)
{
	// 12/20/2024
	// convert decimal latitude & longitude to DDmm.mmN/DDDmm.mmW
	lat = constrain(lat, -90, 90);
	lon = constrain(lon, -180, 180);

	const char *latID = (lat < 0) ? "S" : "N";
	const char *lonID = (lon < 0) ? "W" : "E";
	lat = abs(lat);
	lon = abs(lon);
	uint8_t latDeg = (int)lat;			// the characteristic of lat (degrees)
	float latMin = 60 * (lat - latDeg); // the mantissa of lat (minutes)
	uint8_t lonDeg = (int)lon;
	float lonMin = 60 * (lon - lonDeg);

	char buf[20]; // Increased buffer size to safely accommodate the formatted string
	snprintf(buf, sizeof(buf), "%02u%05.2f%.1s/%03u%05.2f%.1s",
			 latDeg, latMin, latID, lonDeg, lonMin, lonID);
	return String(buf);
} // APRSlocation()

void postToAPRS(String message)
{
	// post a message to APRS-IS
	checkAPRSConnection(); // check if connected to APRS-IS server
	if (client.connected())
	{
		client.println(message);
		DEBUG_PRINTLN("APRS posted: " + message);
	}
	else
	{
		DEBUG_PRINTLN(F("APRS connection lost. Cannot post message."));
	}
}

void processBulletins()
{
	//! process APRS bulletins
	//? Check if it is 0800 EST and the morning bulletin has not been sent
	String bulletinText = "";
	if (myTZ.hour() == 8 && myTZ.minute() == 0 && !amBulletinSent)
	{
		bulletinText = pickAphorism(APHORISM_FILE, lineArray);
		APRSsendBulletin(bulletinText, "M"); // send morning bulletin
		amBulletinSent = true;				 // mark it sent
	}

	//? Check if it is 2000 EST and the evening bulletin has not been sent
	if (myTZ.hour() == 20 && myTZ.minute() == 0 && !pmBulletinSent)
	{
		bulletinText = pickAphorism(APHORISM_FILE, lineArray);
		APRSsendBulletin(bulletinText, "E"); // send evening bulletin
		pmBulletinSent = true;				 // mark it sent
	}

	//? Reset the bulletin flags at midnight if either is true
	static int lastDay = -1;
	int currentDay = myTZ.day();
	if (currentDay != lastDay)
	{
		lastDay = currentDay;
		amBulletinSent = false;
		pmBulletinSent = false;
	}
}

void APRSsendBulletin(String message, String ID)
{
	// send a bulletin or announcement to APRS-IS
	if (message.length() > 67)
	{
		DEBUG_PRINTLN(F("APRS bulletin too long. Max 67 characters."));
		return;
	}

	String bulletin = APRSformatBulletin(message, ID);
	postToAPRS(bulletin);
} // APRSsendBulletin()	
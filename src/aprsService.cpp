/**
 * @file aprsService.cpp
 * @brief Contains implementation functions for posting weather data and bulletins to APRS-IS.
 * @author Karl Berger
 * @date 2025-06-09
 */

#include "aprsService.h"

#include <Arduino.h>		   // Arduino functions
#include "aphorismGenerator.h" // aphorism generator for bulletins
#include "credentials.h"	   // APRS, Wi-Fi and weather station credentials
#include "timeFunctions.h"	   // time functions
#include <WiFiClient.h>		   // APRS connection
#include "wug_debug.h"		   // debug print macro

WiFiClient client; // APRS-IS client connection

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
// #define APRS_SOFTWARE_NAME "D1S-VEVOR"						  // unit ID
#define APRS_SOFTWARE_VERS FW_VERSION // FW version
#define APRS_PORT 14580				  // do not change port
#define APRS_TIMEOUT 2000L			  // milliseconds
const int APRS_BUFFER_SIZE = 513;	  // APRS buffer size, must be at least 512 bytes + 1 for null terminator

// *******************************************************
// ******************* GLOBALS ***************************
// *******************************************************
// APRS Data Type Identifiers
// page 17 http://www.aprs.org/doc/APRS101.PDF
const char APRS_ID_POSITION_NO_TIMESTAMP = '!';
const char APRS_ID_TELEMETRY = 'T';
const char APRS_ID_WEATHER = '_';
const char APRS_ID_MESSAGE = ':';
const char APRS_ID_QUERY = '?';
const char APRS_ID_STATUS = '>';
const char APRS_ID_USER_DEF = '{';
const char APRS_ID_COMMENT = '#';
String APRSdataMessage = "";   // message text
String APRSdataWeather = "";   // weather data
String APRSdataTelemetry = ""; // telemetry data
String APRSserver = "";		   // APRS-IS server
char APRSage[9] = "";		   // time stamp for received data

// Global state tracker
enum APRS_State {
  APRS_DISCONNECTED,
  APRS_CONNECTED,
  APRS_LOGGED_IN,
  APRS_VERIFIED
};
APRS_State aprsState = APRS_DISCONNECTED;

//! ************ APRS Bulletin globals ***************
// int *lineArray;				 // holds shuffled index to aphorisms
int lineCount;				 // number of aphorisms in file
bool amBulletinSent = false; // APRS morning bulletin
bool pmBulletinSent = false; // APRS evening bulletin
int lineIndex = 1;			 // APRS bulletin index

/**
 * @brief Performs the APRS-IS logon procedure.
 *
 * Constructs the APRS-IS logon string using the configured callsign, passcode,
 * software name, version, and filter, then sends it to the APRS-IS server.
 * Also outputs the logon string to the debug interface for logging purposes.
 *
 * Dependencies:
 * - Assumes global variables/constants: CALLSIGN, APRS_PASSCODE, APRS_SOFTWARE_NAME,
 *   APRS_SOFTWARE_VERS, APRS_FILTER, and client are defined and accessible.
 * - Uses DEBUG_PRINTLN for debug output.
 */
void performAPRSLogon() {
    // Construct the APRS-IS logon string
    String dataString = "user " + CALLSIGN;
    dataString += " pass " + APRS_PASSCODE;
    dataString += " ver " + APRS_SOFTWARE_NAME + " " + APRS_SOFTWARE_VERS;
    dataString += " filter " + APRS_FILTER;

    // Send the logon string to the server
    client.println(dataString);
    DEBUG_PRINTLN("APRS logon: " + dataString);
}

/**
 * @brief Establishes a connection to the APRS network and verifies logon status.
 *
 * This function attempts to connect to the APRS network. If the connection is successful,
 * it updates the APRS state to APRS_CONNECTED and performs the APRS logon procedure.
 * After logging on, it verifies the logon status, and if successful, updates the APRS state
 * to APRS_VERIFIED.
 */
void connectToAPRSserver() {
  if (connectToAPRS()) {
    aprsState = APRS_CONNECTED;
    performAPRSLogon();
    if (verifyLogonStatus()) {
      aprsState = APRS_VERIFIED;
    }
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


/**
 * @brief Posts a message to the APRS-IS network.
 *
 * This function sends the specified message to the APRS-IS server if the client is connected.
 * If the connection is lost, it logs a debug message indicating the failure to post.
 *
 * @param message The APRS message to be posted as a String.
 */
void postToAPRS(String message)
{
	// post a message to APRS-IS
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

/**
 * @brief Processes and sends scheduled APRS bulletins.
 *
 * This function checks the current time and sends APRS bulletins at specific times of the day:
 * - At 08:00 EST, if the morning bulletin has not been sent, it selects an aphorism and sends it as a morning bulletin.
 * - At 20:00 EST, if the evening bulletin has not been sent, it selects an aphorism and sends it as an evening bulletin.
 * 
 * The function ensures that each bulletin is sent only once per day by using flags (`amBulletinSent` and `pmBulletinSent`).
 * These flags are reset at midnight to allow bulletins to be sent again the next day.
 *
 * Dependencies:
 * - `myTZ`: An object providing the current time (hour, minute, day).
 * - `pickAphorism()`: Function to select a bulletin message.
 * - `APRSsendBulletin()`: Function to send the bulletin.
 * - `APHORISM_FILE`, `lineArray`: Resources used for selecting aphorisms.
 * - `amBulletinSent`, `pmBulletinSent`: Flags indicating if bulletins have been sent.
 */
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

/**
 * @brief Sends a bulletin or announcement to APRS-IS.
 *
 * This function formats and posts a bulletin message to the APRS-IS network.
 * The message must not exceed 67 characters in length. If the message is too long,
 * the function will print a debug message and return without sending.
 *
 * @param message The bulletin message to send (maximum 67 characters).
 * @param ID The identifier for the bulletin.
 */
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

// *******************************************************
// **************** SEND APRS ACK ************************
// *******************************************************
void APRSsendACK(String recipient, String msgID)
{
	String dataString = CALLSIGN;
	dataString += ">APRS,TCPIP*:";
	dataString += APRS_ID_MESSAGE;
	dataString += APRSpadCall(recipient); // pad to 9 characters
	dataString += APRS_ID_MESSAGE;
	dataString += "ack";
	dataString += msgID;
	client.println(dataString); // send to APRS-IS
	Serial.println(dataString); // print to serial port
} // APRsendACK()

/**
 * @brief Reads an APRS packet from the client connection.
 *
 * This function attempts to read a complete APRS packet from the client connection.
 * It waits for data to become available and reads it until a newline character is encountered.
 * If no data is available within a specified timeout, it closes the connection.
 *
 * @param packet A reference to a String where the read packet will be stored.
 * @return true if a packet was successfully read, false otherwise.
 */
bool readAPRSPacket(String &packet) {
    static unsigned long timeoutStamp = 0;
    const unsigned long TIMEOUT_MS = 1500;

    // If not connected, do not attempt to read
    if (!client.connected()) {
        packet = "";
        return false;
    }

    // If there is data available, read a line
    if (client.available()) {
        packet = client.readStringUntil('\n');
        timeoutStamp = 0; // Reset timer on successful read
        return (packet.length() > 0);
    } else {
        // Start or continue timeout timer
        if (timeoutStamp == 0) {
            timeoutStamp = millis();
        } else if (millis() - timeoutStamp > TIMEOUT_MS) {
            timeoutStamp = 0;
            client.stop(); // Close connection on timeout
        }
        packet = "";
        return false;
    }
}

/**
 * @brief Polls for incoming APRS packets and processes them if available.
 *
 * This function attempts to read an APRS packet into a buffer. If a packet is successfully read,
 * it processes the packet data and outputs the received packet to the serial console.
 *
 * @note Relies on the functions readAPRSPacket(String&) and handleAPRSData(const String&).
 */
void pollAPRS()
{
  if (aprsState != APRS_VERIFIED) return;

  String packet;
  while (readAPRSPacket(packet)) {
    if (packet.startsWith("#")) {  // Handle server messages
    //   processServerMessage(packet);
    } else {                        // Handle APRS data
    //   processAPRSPacket(packet);
    }
  }
  
  // Connection watchdog
  if (!client.connected()) {
    aprsState = APRS_DISCONNECTED;
    DEBUG_PRINTLN(F("Connection lost"));
  }
}

/**
 * @brief Verifies the logon status by reading APRS packets within a timeout period.
 *
 * This function waits for a response from the APRS server indicating the logon status.
 * It reads incoming APRS packets until either a verified or unverified logon response is received,
 * or until the timeout period (APRS_TIMEOUT) elapses.
 *
 * @return true if logon is verified, false if unverified or if the operation times out.
 */
bool verifyLogonStatus() {
    unsigned long timeout = millis() + APRS_TIMEOUT;
    while (millis() < timeout) {
        String response;
        if (readAPRSPacket(response)) {
            // Look for the logon response line
            if (response.startsWith("# logresp")) {
                if (response.indexOf("verified") != -1 && response.indexOf("unverified") == -1) {
                    DEBUG_PRINTLN(F("Logon verified"));
                    return true;
                } else if (response.indexOf("unverified") != -1) {
                    DEBUG_PRINTLN(F("Logon unverified"));
                    return false;
                }
            }
        }
        yield();
    }
    DEBUG_PRINTLN(F("Verification timeout"));
    return false;
}

/**
 * @brief Updates the APRS (Automatic Packet Reporting System) state machine.
 *
 * This function manages the APRS connection and data polling process by
 * transitioning through various states:
 * - If disconnected, attempts to establish a connection.
 * - If connected or logged in, verifies the logon status.
 * - If verified, polls APRS data.
 *
 * The function relies on external state variables and helper functions:
 * - aprsState: Current state of the APRS connection.
 * - establishAPRSConnection(): Initiates connection to APRS.
 * - verifyLogonStatus(): Checks if logon is successful.
 * - pollAPRS(): Polls APRS data when verified.
 */
void updateAPRS() {
  // Update APRS data
  switch (aprsState) {
    case APRS_DISCONNECTED:
      connectToAPRS();
      break;
      
    case APRS_CONNECTED:  // Fall through
    case APRS_LOGGED_IN:
      if (verifyLogonStatus()) {
        aprsState = APRS_VERIFIED;
      }
      break;
      
    case APRS_VERIFIED:
      pollAPRS();
      break;
  }
} // updateAPRS()

/**
 * @brief Attempts to establish a connection to the APRS server.
 *
 * This function checks if the client is already connected to the APRS server.
 * If not connected, it attempts to connect using the specified server address and port.
 * Debug messages are printed to indicate the connection status.
 *
 * @return true if the client is already connected or the connection is successful, false otherwise.
 */
bool connectToAPRS() {
    // Attempt to connect only if not already connected
    if (client.connected()) {
        return true;
    }

    if (client.connect(APRS_SERVER, APRS_PORT)) {
        DEBUG_PRINTLN(F("APRS connected"));
        return true;
    } else {
        DEBUG_PRINTLN(F("APRS connection failed."));
        return false;
    }
}

// end of file
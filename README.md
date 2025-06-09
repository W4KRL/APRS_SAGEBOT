# WUG_APRS_TS
Connects Weather Underground feed to APRS and Thingspeak


What to Include in README.md
- Project Overview – Brief description of what your project does.
- Installation Steps – Any software dependencies or setup requirements.
- Device Connections – A simplified version of wiring or hardware requirements.
- Basic Usage Instructions – How to run the program.
- Links to Further Documentation – If needed, reference a more detailed 


// Displays local weather from VEVOR 7-in-1 Weather Station
// through Weather Underground PWS account
/* Configured in credentials.h
 ? Data Needed:
 *   Wi-Fi credentials (SSID & Password)
 *   WeatherUnderground.com API key
 *   Olson timezone
 *   APRS callsign & SSID
 *   APRS passcode
 *   ThingSpeak channel ID
 */


/*
******************************************************
**************** DEVICE CONNECTIONS ******************
******************************************************
!              WEMOS D1 MINI PINOUTS
 /RST - TFT RST          TX - GPIO 1 not used
   A0 - ADC not used     RX - GPIO 3 not used
   D0 - GPIO 16 CS       D1 - GPIO 5 I2C SCL
   D5 - GPIO 14 SCK      D2 - GPIO 4 I2C SDA
   D6 - GPIO 12 MISO     D3 - GPIO 0 TFT_DC
   D7 - GPIO 13 MOSI     D4 - GPIO 2 LED_BUILTIN
   D8 - GPIO 15 SS      GND - return for sensor
  3V3 - power to sensor  5V - not used
                  |USB|

 ! TFT = WEMOS 1.4 shield connections
 ! Mod to screen moves CS from D4 to D0 to avoid LED flash
 ! Used on kits starting August 2022
  #define TFT_RST -1  // use -1 according to Wemos
  #define TFT_DC D3        // GPIO 0
  #define MOSI D7          // GPIO 13
  #define SCK D5           // GPIO 14
  #define TFT_LED NC       // no connection
*/


// this version accomodates modified and unmodified TFT screens
// it must be compiled as one or the other
// unmarked version has CS = D4
// use version v2 with TFT_MOD CS = D0
// expects AHT10 sensor

//! Weather code from D1S-Display-WBIT-Mod.ino
//! APRS code from D1M-WX2-APRS-TS.ino
//! ThingSpeak code from XXXX

ToDo:

1. Improve updateClocks() to eliminate allowHandMovement and allowNumberFlip booleans.
2. Filter aphorisms.txt to remove extended ASCII characters
3. Adjust UV frame top to split evenly between lines
4. ??? Add SCPI for paramter controls
5. ??? Add Preferences for secure parameters
6. Add Doxygen markup
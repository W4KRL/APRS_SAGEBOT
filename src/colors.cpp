/*
 * colors.cpp
 * 
 * This file defines a set of color constants using 16-bit RGB565 format, 
 * suitable for use in embedded graphics applications (e.g., TFT displays).
 * 
 * Color definitions include:
 *  - Standard resistor color codes (BLACK, BROWN, RED, ORANGE, YELLOW, GREEN, BLUE, VIOLET, GRAY, WHITE, GOLD, SILVER)
 *  - Additional colors and shades (CYAN, MAGENTA, ORANGERED, LIME, LIGHTGRAY, DARKGRAY, SLATEGRAY, DARKSLATEGRAY, LIGHTGREEN, MIDNIGHTBLUE)
 *  - A 12-color palette designed for color blindness accessibility
 * 
 * The file also defines color constants for various UI elements, such as:
 *  - Analog clock frame and dial components
 *  - Digital clock frame and display elements
 *  - Weather display frames
 *  - Almanac display frames
 * 
 * Author: Bruce E. Hall (original color code reference)
 * Reference: http://w8bh.net/pi/rgb565.py
 */
#include "colors.h"

// Color definitions similar to the resistor color code
// By Bruce E. Hall
// http://w8bh.net/pi/rgb565.py
const int BLACK     =  0x0000;
const int BROWN     =  0xA145;
const int RED       =  0xF800;
const int ORANGE    =  0xFD20;
const int YELLOW    =  0xFFE0;
const int GREEN     =  0x07E0;
const int BLUE      =  0x001F;
const int VIOLET    =  0xEC1D;
const int GRAY      =  0x8410;
const int WHITE     =  0xFFFF;
const int GOLD      =  0xFEA0;
const int SILVER    =  0xC618;

// Grays & others
const int CYAN          =  0x07FF;
const int MAGENTA       =  0xF81F;
const int ORANGERED     =  0xFA20;
const int LIME          =  0x07E0;
const int LIGHTGRAY     =  0xD69A;
const int DARKGRAY      =  0xAD55;
const int SLATEGRAY     =  0x7412;
const int DARKSLATEGRAY =  0x2A69;
const int LIGHTGREEN    =  0x9772;
const int MIDNIGHTBLUE  =  0x18CE;
const int DARKBLUE      =  0x6C5D;

// 12-COLOR PALLETE FOR COLOR BLINDNESS
const int JAZZBERRY_JAM     = 0x980C;
const int JEEPERS_CREEPERS  = 0x04F0;
const int BARBIE_PINK       = 0xFAD5;
const int AQUAMARINE        = 0x07F9;
const int FRENCH_VIOLET     = 0x8019;
const int DODGER_BLUE       = 0x047F;
const int CAPRI             = 0x061F;
const int PLUM              = 0xFD9F;
const int CARMINE           = 0xA004;
const int ALIZARIN_CRIMSON  = 0xE006;
const int OUTRAGEOUS_ORANGE = 0xFB67;
const int BRIGHT_SPARK      = 0xFE07;

// *******************************************************
// ************ FRAME COLOR CHOICES **********************
// *******************************************************

// ANALOG CLOCK FRAME
const int C_ANALOG_FRAME_EDGE    = RED;
const int C_ANALOG_FRAME_BG      = GRAY;
const int C_ANALOG_DIAL_BG       = WHITE;
const int C_ANALOG_TZ            = YELLOW;      // timezone & AM/PM text
const int C_ANALOG_DIAL_EDGE     = BLACK;
const int C_ANALOG_AM_NUMERALS   = BLACK;
const int C_ANALOG_PM_NUMERALS   = ORANGERED;   // PM numerals
const int C_ANALOG_DIAL_TICKS    = BLACK;
const int C_ANALOG_SEC_HAND      = ORANGERED;
const int C_ANALOG_MIN_HAND      = BLUE;
const int C_ANALOG_HOUR_HAND     = GREEN;
const int C_ANALOG_HUB           = BLACK;       // prints over hands
const int C_ANALOG_INDOOR        = RED;

// DIGITAL CLOCK FRAME
const int C_DIGITAL_FRAME_EDGE  = WHITE;
const int C_DIGITAL_BG          = BLACK;
const int C_DIGITAL_ALT_TZ      = ORANGERED;  // UTC
const int C_DIGITAL_LOCAL_TZ    = LIME;       // local timezone
const int C_DIGITAL_INDOOR      = YELLOW;     // Indoor temp & humid

// WEATHER FRAMES
const int C_WX_TOP_BG           = YELLOW;
const int C_WX_TOP_TEXT         = BLUE;
const int C_WX_BOTTOM_BG        = BLUE;
const int C_WX_BOTTOM_TEXT      = YELLOW;

// ALMANAC FRAME
const int C_ALM_TOP_BG          = MIDNIGHTBLUE;
const int C_ALM_TOP_TEXT        = YELLOW;
const int C_ALM_BOTTOM_BG       = YELLOW;
const int C_ALM_BOTTOM_TEXT     = BLUE;
const int C_ALM_MOON_BG         = DARKBLUE;
const int C_ALM_MOON_FACE       = WHITE;
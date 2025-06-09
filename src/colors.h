/**
 * @file colors.h
 * @brief Color definitions for the TFT display.
 * @details This file contains the color definitions for the TFT display.
 *          The colors are defined in RGB565 format using the 16-bit hex values.
 * @author Karl Berger
 * @date 2025-05-18
 */

#ifndef COLORS_H
#define COLORS_H

// *******************************************************
// ****************** colors.h ***************************
// *******************************************************

// This configuration file must reside in the same Arduino
// directory as the remote display sketch

// Colors must use 16-bit definitions in RGB565 format
// https://ee-programming-notepad.blogspot.com/2016/10/16-bit-color-generator-picker.html
// tool for RGB565 to 16-bit hex
// http://www.rinkydinkelectronics.com/calc_rgb565.php

// Color definitions similar to the resistor color code
// By Bruce E. Hall
// http://w8bh.net/pi/rgb565.py
extern const int BLACK;
// extern const int BROWN;
extern const int RED;
// extern const int ORANGE;
extern const int YELLOW;
extern const int GREEN;
extern const int BLUE;
// extern const int VIOLET;
// extern const int GRAY;
extern const int WHITE;
// extern const int GOLD;
// extern const int SILVER;

// Grays & others
// extern const int CYAN;
extern const int MAGENTA;
// extern const int ORANGERED;
// extern const int LIME;
// extern const int LIGHTGRAY;
// extern const int DARKGRAY;
// extern const int SLATEGRAY;
// extern const int DARKSLATEGRAY;
// extern const int LIGHTGREEN;
// extern const int MIDNIGHTBLUE;

// 12-COLOR PALLETE FOR COLOR BLINDNESS
// extern const int JAZZBERRY_JAM;
// extern const int JEEPERS_CREEPERS;
// extern const int BARBIE_PINK;
// extern const int AQUAMARINE;
// extern const int FRENCH_VIOLET;
// extern const int DODGER_BLUE;
// extern const int CAPRI;
// extern const int PLUM;
// extern const int CARMINE;
extern const int ALIZARIN_CRIMSON;
extern const int OUTRAGEOUS_ORANGE;
// extern const int BRIGHT_SPARK;

// *******************************************************
// ************ FRAME COLOR CHOICES **********************
// *******************************************************

// ANALOG CLOCK FRAME
extern const int C_ANALOG_FRAME_EDGE;
extern const int C_ANALOG_FRAME_BG;
extern const int C_ANALOG_DIAL_BG;
extern const int C_ANALOG_TZ;      // timezone & AM/PM text
extern const int C_ANALOG_DIAL_EDGE;
extern const int C_ANALOG_AM_NUMERALS;
extern const int C_ANALOG_PM_NUMERALS;
extern const int C_ANALOG_DIAL_TICKS;
extern const int C_ANALOG_SEC_HAND;
extern const int C_ANALOG_MIN_HAND;
extern const int C_ANALOG_HOUR_HAND;
extern const int C_ANALOG_HUB;       // prints over hands
extern const int C_ANALOG_INDOOR;

// DIGITAL CLOCK FRAME
extern const int C_DIGITAL_FRAME_EDGE;
extern const int C_DIGITAL_BG;
extern const int C_DIGITAL_ALT_TZ;  // UTC
extern const int C_DIGITAL_LOCAL_TZ;       // local timezone
extern const int C_DIGITAL_INDOOR;     // Indoor temp & humid

// WEATHER FRAMES
extern const int C_WX_TOP_BG;
extern const int C_WX_TOP_TEXT;
extern const int C_WX_BOTTOM_BG;
extern const int C_WX_BOTTOM_TEXT;

// ALMANAC FRAME
extern const int C_ALM_TOP_BG;
extern const int C_ALM_TOP_TEXT;
extern const int C_ALM_BOTTOM_BG;
extern const int C_ALM_BOTTOM_TEXT;
extern const int C_ALM_MOON_BG;
extern const int C_ALM_MOON_FACE;

#endif // COLORS_H
// End of file

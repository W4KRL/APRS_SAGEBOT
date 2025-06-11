/**
 * @file tftDisplay.cpp
 * @author Karl W. Berger
 * @date 2025-06-02
 * @brief TFT display implementation
 */

#include "tftDisplay.h"

#include <Arduino.h> // for Arduino functions
#include <TFT_eSPI.h> // for TFT_eSPI library
#include "Roboto_Bold_12.h" // for Roboto Bold 12 font
#include "Roboto_Bold_16.h" // for Roboto Bold 16 font

// Display configuration
// Display dimensions (initialized in setup)
const int USB_LEFT = 0;
const int USB_DOWN = 1;
const int USB_RIGHT = 2;
const int USB_UP = 3;
const int LEFT_COL = 6;
const int RIGHT_COL = 125;
const int HEADER_RAD = 8;
const int HEADER_Y = 36;

const int FRAME_ORIENTATION = USB_LEFT;

//! Display instantiation
TFT_eSPI tft;

// Font resources
const GFXfont *SmallBold = &Roboto_Bold_12;
const GFXfont *LargeBold = &Roboto_Bold_16;

int SCREEN_W = 128;
int SCREEN_H = 128;
int SCREEN_W2 = SCREEN_W / 2;
int SCREEN_H2 = SCREEN_H / 2;

void setupTFTDisplay()
{
  tft.init();
  tft.setRotation(FRAME_ORIENTATION);

  // Update dimensions based on actual display
  SCREEN_W = tft.width();
  SCREEN_H = tft.height();
  SCREEN_W2 = SCREEN_W / 2;
  SCREEN_H2 = SCREEN_H / 2;
}

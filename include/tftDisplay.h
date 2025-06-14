/**
 * @file tftDisplay.h
 * @brief TFT display configuration and dimensions
 * @author Karl Berger
 * @date 2025-05-14
 */

#ifndef TFT_DISPLAY_H
#define TFT_DISPLAY_H

#include <TFT_eSPI.h>

//! TFT instance
extern TFT_eSPI tft;

//! Display constants
extern const int LEFT_COL;    // left margin
extern const int RIGHT_COL;   // right margin
extern const int HEADER_RAD;  // header corner radius
extern const int HEADER_Y;    // header bottom row

//! values read from display device
extern int SCREEN_W;    // width in pixels
extern int SCREEN_H;    // height in pixels
extern int SCREEN_W2;   // half width
extern int SCREEN_H2;   // half height

//! Font pointers
extern const GFXfont* SmallBold;
extern const GFXfont* LargeBold;

//! Functions
void setupTFTdisplay();

#endif // TFT_DISPLAY_H

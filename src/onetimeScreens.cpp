/**
 * @file onetimeScreens.cpp
 * @brief Functions to display one-time screens on the TFT display
 * @date 2025-05-23
 */

#include "onetimeScreens.h"

#include <Arduino.h>
#include "tftDisplay.h"
#include "credentials.h"
#include "colors.h"
#include "timeFunctions.h"

/*
******************************************************
******************* Splash Screen ********************
******************************************************
*/
void splashScreen()
{
  // 2025-05-23
  tft.setFreeFont(LargeBold);
  tft.fillScreen(BLUE);
  tft.setTextColor(YELLOW);
  int textHeight = tft.fontHeight();
  int lineSpacing = 2;
  int row[5];
  row[0] = 16;
  for (int i = 1; i < 5; i++)
  {
    row[i] = row[i - 1] + textHeight + lineSpacing;
  }

  tft.setTextDatum(TC_DATUM); // font top, centered
  tft.drawString("D1S-WUG", SCREEN_W2, row[0]);
  tft.drawString("Display", SCREEN_W2, row[1]);
  tft.drawString("by", SCREEN_W2, row[2]);
  tft.drawString("IoT Kits", SCREEN_W2, row[3]);
  tft.drawString("v" + FW_VERSION + "-M", SCREEN_W2, row[4]);
  // Decorative frame
  for (int i = 0; i < 4; i++)
  {
    tft.drawRoundRect(12 - 3 * i, 12 - 3 * i, SCREEN_W - 12, SCREEN_H - 12, 8, YELLOW);
  }
} // splashScreen()


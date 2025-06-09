/**
 * @file onetimeScreens.cpp
 * @brief Functions to display one-time screens on the TFT display
 * @date 2025-05-23
 */

#include "onetimeScreens.h"

#include <Arduino.h>
#include "tftDisplay.h"
#include "credentials.h"
#include "weatherService.h"
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
  tft.unloadFont();
} // splashScreen()

/*
******************************************************
****************** Data Screen ***********************
******************************************************
*/
void dataScreen()
{
  // 2025-05-23
  tft.fillScreen(BLACK); // clear screen
  String units = (METRIC_DISPLAY) ? "Metric" : "Imperial";
  String clock;
  if (ANALOG_CLOCK && DIGITAL_CLOCK)
  {
    clock = "Analog&Digital";
  }
  else if (ANALOG_CLOCK && !DIGITAL_CLOCK)
  {
    clock = "Analog";
  }
  else if (!ANALOG_CLOCK && DIGITAL_CLOCK)
  {
    clock = "Digital";
  }
  else
  {
    clock = "No";
  }
  clock += " Clock";

  tft.setFreeFont(SmallBold);
  tft.setTextDatum(TC_DATUM); // center text
  tft.setTextColor(YELLOW);

  int textHeight = tft.fontHeight(); // font height in pixels
  int lineSpacing = 1;               // line spacing in pixels
  int row[9];                        // array to hold rows
  row[0] = 0;                        // top row
  for (int i = 1; i < 9; i++)
  {
    row[i] = row[i - 1] + textHeight + lineSpacing;
  }

  tft.drawString("Location:", SCREEN_W2, row[0]);
  tft.drawString(String(wx.obsLat, 2) + "/" + String(wx.obsLon, 2), SCREEN_W2, row[1]);
  tft.drawString(wx.obsNeighborhood, SCREEN_W2, row[2]);
  tft.setTextColor(GREEN);
  tft.drawString("Time Zone:" + getTimezoneName(), SCREEN_W2, row[3]);
  tft.drawString(MY_TIMEZONE, SCREEN_W2, row[4]); // Olsen timezone
  tft.setTextColor(WHITE);
  tft.drawString("Units & Duration:", SCREEN_W2, row[5]);
  tft.drawString(units + " " + SCREEN_DURATION + " seconds", SCREEN_W2, row[6]);
  tft.drawString(clock, SCREEN_W2, row[7]);
  tft.setTextColor(RED);
  tft.drawString(APHORISM_FILE, SCREEN_W2, row[8]);

  tft.unloadFont();
} // dataScreen()
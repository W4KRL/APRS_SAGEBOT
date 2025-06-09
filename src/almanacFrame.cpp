/**
 * @file    almanacFrame.cpp
 * @author  Karl Berger
 * @date    2025-06-05
 * @brief   Draws the Almanac frame on the TFT display.
 *
 * This function displays the current date, sunrise and sunset times,
 * and moon phase information on the TFT display. It formats and draws
 * the following elements:
 *   - Frame panels with appropriate background colors.
 *   - "Almanac" title and current date.
 *   - Sunrise and sunset times, adjusted for timezone offset.
 *   - Moon phase graphic and phase name.
 *
 * Dependencies:
 *   - Requires global weather data (wx) for sunrise/sunset times.
 *   - Uses TFT display functions for drawing text and graphics.
 *   - Utilizes timezone, unit conversion, and lunar phase utilities.
 *
 * No parameters or return value.
 */
#include "almanacFrame.h" // almanac frame

#include <Arduino.h>		  // Arduino functions
#include "colors.h"			  // for colors
#include "lunation.h"		  // for findPhaseName()
#include "sequentialFrames.h" // for drawFramePanels()
#include "tftDisplay.h"		  // for TFT display functions
#include "timeFunctions.h"	  // for getOffset()
#include "unitConversions.h"  // for moonPhase()
#include "weatherService.h"	  // for weather data

void almanacFrame()
{
	char sunRiseTime[11] = "";
	char sunSetTime[11] = "";
	int hh, mm;
	unsigned long adjustedTime;

	// Process sunrise time
	adjustedTime = (wx.forSunRise - 60 * getOffset()) % 86400UL;
	hh = adjustedTime / 3600;
	mm = (adjustedTime % 3600) / 60;
	hh = to12HourFormat(hh);
	snprintf(sunRiseTime, sizeof(sunRiseTime), "Rise%2d:%02da", hh, mm);

	// Process sunset time
	adjustedTime = (wx.forSunSet - 60 * getOffset()) % 86400UL;
	hh = adjustedTime / 3600;
	mm = (adjustedTime % 3600) / 60;
	hh = to12HourFormat(hh);
	snprintf(sunSetTime, sizeof(sunSetTime), "Set %2d:%02dp", hh, mm);

	// Process moon phase
	float fraction = moonPhase();				  // find moon phase
	phaseName moonName = findPhaseName(fraction); // returns two Strings

	// Draw the Almanac frame
	drawFramePanels(C_ALM_TOP_BG, C_ALM_BOTTOM_BG);
	tft.setTextColor(C_ALM_TOP_TEXT);
	tft.setTextDatum(TC_DATUM); // centered text
	tft.setFreeFont(LargeBold);

	int row[7]; // row positions in pixels for text
	int textHeight = tft.fontHeight();
	int lineSpacing = -1; // squeeze the lines together
	row[0] = 1;			  // top row
	for (int i = 1; i < 7; i++)
	{
		row[i] = row[i - 1] + textHeight + lineSpacing;
	}

	tft.drawString("Almanac", SCREEN_W2, row[0]);
	tft.drawString(dateTime("D M j"), SCREEN_W2, row[1]); // "Sat Aug 7"
	tft.setTextColor(C_ALM_BOTTOM_TEXT);
	tft.drawString("SUN", SCREEN_W2, row[2]);
	tft.drawString(sunRiseTime, SCREEN_W2, row[3]);
	tft.drawString(sunSetTime, SCREEN_W2, row[4]);

	// panel for moon data
	tft.fillRect(0, 90, SCREEN_W, SCREEN_H - 90, C_ALM_MOON_BG);
	// display moon phase and name
	tft.setTextColor(C_ALM_TOP_TEXT, C_ALM_MOON_BG);
	tft.setTextDatum(TR_DATUM); // flush right
	drawMoonFace(20, SCREEN_H - 19, 15, fraction, C_ALM_MOON_FACE, C_ALM_MOON_BG);
	tft.drawString(moonName.firstWord, SCREEN_W - 6, row[5]);  // 93
	tft.drawString(moonName.secondWord, SCREEN_W - 6, row[6]); // 110
	tft.unloadFont();
} // almanacFrame()
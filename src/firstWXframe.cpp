#include "firstWXframe.h"

#include <Arduino.h>		  // for Arduino functions
#include "tftDisplay.h"		  // for TFT display functions
#include "weatherService.h"	  // for weather data
#include "unitConversions.h"  // for CtoF(), MMtoIN(), getRainIntensity()
#include "colors.h"			  // for colors
#include "credentials.h"	  // for METRIC_DISPLAY
#include "sequentialFrames.h" // for drawFramePanels();

/*
******************************************************
************* FIRST WEATHER FRAME ********************
******************************************************
*/
void firstWXframe()
{
	// 12/04/2024 fixed UV number display

	// prepare values
	String dispTempNow = (METRIC_DISPLAY) ? String(wx.obsTemp, 1) + " C" : String(CtoF(wx.obsTemp), 0) + " F";
	float feelTempC = (wx.obsTemp > 18) ? wx.obsHeatIndex : wx.obsWindChill;
	String dispTempFeel = (METRIC_DISPLAY) ? String(feelTempC, 0) + " C" : String(CtoF(feelTempC), 0) + " F";
	String dispPrecipType = "Rain";
	String dispPrecipAmt = (METRIC_DISPLAY) ? String(wx.obsPrecipTotal, 0) + " mm" : String(MMtoIN(wx.obsPrecipTotal), 2) + " in"; // total today
	String dispPrecipRate = getRainIntensity(wx.obsPrecipRate);																	   // rate in words
	// https://www.epa.gov/sunsafety/uv-index-scale-0
	String uvLabel = "";
	int uvText = YELLOW;	   // text color
	int uvBG = 0;			   // background color
	int uvi = round(wx.obsUV); // round and convert to int

	switch (uvi)
	{
	case -1: // No report or unavailable
		uvLabel = "N/A";
		uvBG = BLACK;
		uvText = WHITE;
		break;

	case 0:
	case 1:
	case 2: // Low risk
		uvLabel = "Low";
		uvBG = GREEN;
		uvText = BLACK;
		break;

	case 3:
	case 4:
	case 5: // Moderate risk
		uvLabel = "Moderate";
		uvBG = YELLOW;
		uvText = BLUE;
		break;

	case 6:
	case 7: // High risk
		uvLabel = "High";
		uvBG = OUTRAGEOUS_ORANGE;
		break;

	case 8:
	case 9:
	case 10:					 // Very High risk
		uvLabel = "Very Hi";	 // to fit display
		uvBG = ALIZARIN_CRIMSON; // ORANGERED;
		break;

	default: // Extreme (uvi >= 11)
		uvLabel = "Extreme";
		uvBG = MAGENTA;
		break;
	}

	// Set font
	tft.setTextColor(C_WX_TOP_TEXT);
	tft.setTextDatum(TC_DATUM); // center text
	tft.setFreeFont(LargeBold);

	int row[7];
	int textHeight = tft.fontHeight();
	int lineSpacing = -1; // squeeze the lines together
	row[0] = 1;			  // top row
	for (int i = 1; i < 7; i++)
	{
		row[i] = row[i - 1] + textHeight + lineSpacing;
	}

	// print labels, values, abd units
	drawFramePanels(C_WX_TOP_BG, C_WX_BOTTOM_BG);

	tft.drawString("Weather", SCREEN_W2, row[0]);
	if (tft.textWidth(wx.forPhraseLong) < SCREEN_W)
	{														 // Will the long phrase fit on the screen?
		tft.drawString(wx.forPhraseLong, SCREEN_W2, row[1]); // If yes, print long phrase in large font
	}
	else
	{								// else
		tft.setFreeFont(SmallBold); // Change to small font
		if (tft.textWidth(wx.forPhraseLong) < SCREEN_W)
		{														 // Will the long phrase fit on the screen?
			tft.drawString(wx.forPhraseLong, SCREEN_W2, row[1]); // If yes, print the long phrase in small font
		}
		else
		{														  // else
			tft.setFreeFont(LargeBold);							  // Change to large font - we know short will fit
			tft.drawString(wx.forPhraseShort, SCREEN_W2, row[1]); // Print the short phrase in the large font
		}
	}

	// print the labels flushed left
	tft.setFreeFont(LargeBold); // restore font if changed in line 2
	tft.setTextDatum(TL_DATUM); // flush left
	tft.setTextColor(YELLOW);
	tft.drawString("Temp", LEFT_COL, row[2]);
	tft.drawString("Feel", LEFT_COL, row[3]);
	tft.drawString(dispPrecipType, LEFT_COL, row[4]);

	// print the data flushed right
	tft.setTextDatum(TR_DATUM); // flush right
	tft.drawString(dispTempNow, RIGHT_COL, row[2]);
	tft.drawString(dispTempFeel, RIGHT_COL, row[3]);
	tft.drawString(dispPrecipAmt, RIGHT_COL, row[4]);
	tft.drawString(dispPrecipRate, RIGHT_COL, row[5]);

	// panel for UV index, backgound color varies with UV index
	// top is straight, bottom is rounded
	// overlay a rounded rectangle
	tft.fillRect(0, 109, SCREEN_W, HEADER_RAD, uvBG);
	tft.fillRoundRect(0, SCREEN_H - 2 * HEADER_RAD, SCREEN_W, 2 * HEADER_RAD, HEADER_RAD, uvBG);
	tft.setTextColor(uvText);
	tft.setTextDatum(TL_DATUM); // flush left
	tft.drawString("UV " + (String)uvi, LEFT_COL, row[6]);
	tft.setTextDatum(TR_DATUM); // flush right
	tft.drawString(uvLabel, RIGHT_COL, row[6]);

	tft.unloadFont();
} // firstWXframe()


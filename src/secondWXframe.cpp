#include "firstWXframe.h"

#include <Arduino.h>		  // for Arduino functions
#include "tftDisplay.h"		  // for TFT display functions
#include "weatherService.h"	  // for weather data
#include "unitConversions.h"  // for CtoF(), MMtoIN(), getWindDirection()
#include "colors.h"			  // for colors
#include "sequentialFrames.h" // for drawFramePanels()
#include "credentials.h"	  // for METRIC_DISPLAY
#include "sequentialFrames.h" // for drawFramePanels();

/*
******************************************************
*************** SECOND WEATHER FRAME *****************
******************************************************
*/
void secondWXframe()
{
	// 11/24/2024

	// calculate values
	// use 1 decimal for Celsius, none for Fahrenheit
	String tempMax = (METRIC_DISPLAY) ? String(wx.forTempMax, 1) : String(CtoF(wx.forTempMax), 0);
	String tempMin = (METRIC_DISPLAY) ? String(wx.forTempMin, 1) + "°C" : String(CtoF(wx.forTempMin), 0) + "°F";
	String dispWindSpeed = (METRIC_DISPLAY) ? String(wx.obsWindSpeed, 0) + " kph" : String(KMtoMILES(wx.obsWindSpeed), 0) + " mph";
	String dispGust = (METRIC_DISPLAY) ? String(wx.obsWindGust, 0) + " kph" : String(KMtoMILES(wx.obsWindSpeed), 0) + " mph";
	String dispSLP = (METRIC_DISPLAY) ? String(wx.obsPressure, 0) + " mb" : String(HPAtoINHG(wx.obsPressure), 2) + " in";
	String dispCloud = String(wx.forCloud) + "%";
	String dispHumid = String(wx.obsHumidity, 0) + "%";

	// print labels, values, and units
	drawFramePanels(C_WX_TOP_BG, C_WX_BOTTOM_BG);

	tft.setTextColor(C_WX_TOP_TEXT);
	tft.setFreeFont(LargeBold);
	tft.setTextDatum(TC_DATUM);

	int row[7];
	int textHeight = tft.fontHeight();
	int lineSpacing = -1; // squeeze the lines together
	row[0] = 1;			  // top row
	for (int i = 1; i < 7; i++)
	{
		row[i] = row[i - 1] + textHeight + lineSpacing;
	}

	tft.drawString("Forecast Hi/Lo", SCREEN_W2, row[0]); // high and low temperature forecasts
	tft.drawString(tempMax + "/" + tempMin, SCREEN_W2, row[1]);

	// labels flushed left  // labels flushed left
	tft.setTextColor(C_WX_BOTTOM_TEXT);
	tft.setTextDatum(TL_DATUM);
	tft.drawString("Gust", LEFT_COL, row[3]);
	tft.drawString("Cloud", LEFT_COL, row[4]);
	tft.drawString("BP", LEFT_COL, row[5]);
	tft.drawString("Humid", LEFT_COL, row[6]);

	// values flushed right
	tft.setTextDatum(TR_DATUM);
	tft.drawString(getCompassDirection(wx.obsWindDir) + dispWindSpeed, RIGHT_COL, row[2]);
	tft.drawString(dispGust, RIGHT_COL, row[3]);
	tft.drawString(dispCloud, RIGHT_COL, row[4]);
	tft.drawString(dispSLP, RIGHT_COL, row[5]);
	tft.drawString(dispHumid, RIGHT_COL, row[6]);

	tft.unloadFont(); // save memory
} // secondWXframe()
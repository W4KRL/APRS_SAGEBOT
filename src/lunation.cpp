/**
 * @file lunation.cpp
 * @author Karl W. Berger
 * @date 2025-06-02
 * @brief Functions for calculating and displaying the lunar phase.
 *
 * This file contains functions to:
 * - Determine the moon phase name based on the fraction of the lunar cycle.
 * - Calculate the current phase of the moon as a fraction of the lunation.
 * - Draw a graphical representation of the moon's face for the current phase.
 *
 * Functions:
 * - phaseName findPhaseName(float fraction): Returns the name of the moon phase for a given fraction of the lunar cycle.
 * - float moonPhase(): Calculates the current phase of the moon as a fraction (0=new, 0.25=first quarter, 0.5=full, 1=new).
 * - void drawMoonFace(int xc, int yc, int r, float fract, int liteColor, int darkColor): Draws the moon face on a TFT display at the specified coordinates, radius, phase fraction, and colors.
 *
 * Dependencies:
 * - tftDisplay.h: For drawing on the TFT display.
 * - timeFunctions.h: For time-related functions.
 * - colors.h: For color definitions.
 */
#include "lunation.h"	

#include <Arduino.h>
#include "tftDisplay.h"	
#include "timeFunctions.h"
#include "colors.h"

/*
*****************************************************
*************** Find Moon Phase Name ****************
*****************************************************
*/
phaseName findPhaseName(float fraction)
{
  // process moon phase
  phaseName name;               // struct for two Strings
  float age = fraction * 29.53; // days in synodic month
  String first[9] = {"New", "Waxing", "First", "Waxing",
                     "Full", "Waning", "Last", "Waning", "New"};
  String second[9] = {"Moon", "Crescent", "Quarter", "Gibbous",
                      "Moon", "Gibbous", "Quarter", "Crescent", "Moon"};
  // Start age for each phase. New, First Quarter, Full, and Last Quarter
  // last 1 day, all others last 6.38 days
  float phaseStart[10] = {0, 0.5, 6.88, 8.88,
                          14.26, 15.26, 21.64, 22.64, 29.03, 30};
  int i = 0; // must be declared outside of for loop for scope
  for (i = 0; i < 9; i++)
  {
    if (age >= phaseStart[i] && age < phaseStart[i + 1])
    {
      break; // i is now the index for the phase text array
    }
  }
  name.firstWord = first[i];
  name.secondWord = second[i];
  return name;
} // findPhaseName()

/*
******************************************************
******************* Moon Phase ***********************
******************************************************
*/
float moonPhase()
{
  // adapted by K. W. Berger from
  // https://minkukel.com/en/various/calculating-moon-phase/
  // two decimal precision is sufficient as lunar month is variable
  // https://en.wikipedia.org/wiki/Lunar_phase
  unsigned long unixTime = now();                         // UTC unix time
  float lunarDays = 29.53059;                             // average days between new moons (synodic)
  unsigned long lunarSecs = lunarDays * 24 * 60 * 60;     // seconds between new moons
  unsigned long new2000 = 947182440;                      // new moon January 6, 2000 18:14 UTC
  unsigned long totalSecs = unixTime - new2000;           // seconds since January 2000 new moon
  unsigned long currentSecs = fmod(totalSecs, lunarSecs); // seconds since most recent new moon
  float fraction = (float)currentSecs / (float)lunarSecs; // fraction of lunation 0=new, 0.25=1Q, 0.5 is full, 1=new

  return fraction;
} // moonPhase()

/*
******************************************************
****************** Draw Moon Face ********************
******************************************************
*/
void drawMoonFace(int xc, int yc, int r, float fract, int liteColor, int darkColor)
{
  // draws a moon face at xc, yc, radius r, fraction of lunation, and colors
  // fract: 0=new, 0.25=first quarter, 0.5=full, 0.75=last quarter, 1=new
  // liteColor: color of the illuminated part
  // darkColor: color of the shadowed part

  int face = 0;
  float f = 0;
  if (fract >= 0 && fract <= 0.25)
  {
    face = 1; // waxing crescent
    f = -4 * fract + 1;
  }
  else if (fract > 0.25 && fract <= 0.5)
  {
    face = 2; // waxing gibbous
    f = 4 * fract - 1;
  }
  else if (fract > 0.5 && fract <= 0.75)
  {
    face = 3; // waning gibbous
    f = -4 * fract + 3;
  }
  else
  {
    face = 4; // waning crescent
    f = 4 * fract - 3;
  }

  int hh = r * r;              // height squared
  int wwe = (f * r) * (f * r); // ellipse half-width squared
  int wwm = r * r;             // moon half-width squared
  int hhwwe = hh * wwe;        // for ellipse
  int hhwwm = hh * wwm;        // for moon

  int x0e = f * r; // half-width of ellipse starting value
  int x0m = r;     // half width of moon disk starting value
  int dxe = 0;     // ellipse x value
  int dxm = 0;     // moon x value

  for (int y = 0; y <= r; y++)
  {
    // ellipse
    int ySquaredWwe = y * y * wwe;
    int x1e = sqrt((hhwwe - ySquaredWwe) / hh);
    dxe = x0e - x1e;
    x0e = x1e;

    // moon
    int ySquaredWwm = y * y * wwm;
    int x1m = sqrt((hhwwm - ySquaredWwm) / hh);
    dxm = x0m - x1m;
    x0m = x1m;

    int xl = xc - x0m; // left end of left line
    int xr = xc + x0m; // right end of right line
    int xp;            // point between left and right lines

    switch (face)
    {
    case 1: // waxing crescent
      xp = xc + x0e;
      tft.drawFastHLine(xl, yc - y, xp - xl, darkColor);
      tft.drawFastHLine(xp, yc - y, xr - xp, liteColor);
      tft.drawFastHLine(xl, yc + y, xp - xl, darkColor);
      tft.drawFastHLine(xp, yc + y, xr - xp, liteColor);
      break;
    case 2: // waxing gibbous
      xp = xc - x0e;
      tft.drawFastHLine(xl, yc - y, xp - xl, liteColor);
      tft.drawFastHLine(xp, yc - y, xr - xp, darkColor);
      tft.drawFastHLine(xl, yc + y, xp - xl, liteColor);
      tft.drawFastHLine(xp, yc + y, xr - xp, darkColor);
      break;
    case 3: // waning gibbous
      xp = xc + x0e;
      tft.drawFastHLine(xl, yc - y, xp - xl, darkColor);
      tft.drawFastHLine(xp, yc - y, xr - xp, liteColor);
      tft.drawFastHLine(xl, yc + y, xp - xl, darkColor);
      tft.drawFastHLine(xp, yc + y, xr - xp, liteColor);
      break;
    case 4: // waning crescent
      xp = xc - x0e;
      tft.drawFastHLine(xl, yc - y, xp - xl, liteColor);
      tft.drawFastHLine(xp, yc - y, xr - xp, darkColor);
      tft.drawFastHLine(xl, yc + y, xp - xl, liteColor);
      tft.drawFastHLine(xp, yc + y, xr - xp, darkColor);
      break;
    default:
      break;
    }
  }
}
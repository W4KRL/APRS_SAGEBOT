/**
 * @file sequentialFrames.cpp
 * @author Karl Berger
 * @date 2025-06-04
 * @brief Sequential frames for the TFT display
 * @details This file contains the functions for displaying sequential frames
 *          on the TFT display. The frames include weather data, almanac data,
 *          and clock data.
 */

#include "sequentialFrames.h"

#include <Arduino.h>       // for Arduino functions
#include "tftDisplay.h"    // for TFT display functions
#include "colors.h"        // for colors
#include "credentials.h"   // for ANALOG_CLOCK and DIGITAL_CLOCK
#include "firstWXframe.h"  // for first weather frame
#include "secondWXframe.h" // for second weather frame
#include "almanacFrame.h"  // for almanac frame
#include "analogClock.h"   // for analog clock frame
#include "digitalClock.h"  // for digital clock frame
#include "taskControl.h"   // for tmrSecondTick to update clocks

// If either ANALOG_CLOCK or DIGITAL_CLOCK is enabled, maxFrames is set to 4.
// Otherwise, maxFrames is set to 3.
int maxFrames = (ANALOG_CLOCK || DIGITAL_CLOCK) ? 4 : 3;

/**
 * @brief Updates the clock display based on the current clock mode.
 *
 * This function checks whether the analog or digital clock mode is active,
 * and triggers a partial update of the corresponding clock frame.
 * - If ANALOG_CLOCK is enabled, updates the analog clock display.
 * - If DIGITAL_CLOCK is enabled, updates the digital clock display.
 *
 * The update is performed as a partial refresh to optimize performance.
 * @note If both ANALOG_CLOCK and DIGITAL_CLOCK are true, the digital clock will be displayed.
 */
void updateClock()
{
  // Update the clock hands and display
  if (ANALOG_CLOCK)
  {
    analogClockFrame(false); // Partial update
  }
  else if (DIGITAL_CLOCK)
  {
    digitalClockFrame(false); // Partial update
  }
} // updateClock()

/**
 * @brief Updates and displays the next sequential frame on the display.
 *
 * This function cycles through a set of predefined frames (weather, almanac, and clock)
 * each time it is called. It increments the current frame index, wraps around to the first
 * frame after the last, and displays the corresponding frame. When switching to the clock
 * frame, it restarts the second tick timer and draws either a digital or analog clock
 * depending on configuration. The second tick timer is stopped during other frames to
 * prevent clock updates.
 *
 * Frames:
 *   1. Weather frame 1
 *   2. Weather frame 2
 *   3. Almanac frame
 *   4. Clock frame (digital or analog)
 *
 * Assumes the existence of:
 *   - maxFrames: total number of frames to cycle through
 *   - tmrSecondTick: timer object for second ticks
 *   - DIGITAL_CLOCK, ANALOG_CLOCK: configuration flags
 *   - firstWXframe(), secondWXframe(), almanacFrame(), 
 *     digitalClockFrame(), analogClockFrame(): frame rendering functions
 */
void updateSequentialFrames()
{
  static int currentFrame = 0; // Tracks which frame is active
  // Increment frame, reset to 1 if exceeds maxFrames
  currentFrame = currentFrame < maxFrames ? currentFrame + 1 : 1;
  tmrSecondTick.stop(); // Stop second tick timer to prevent clock when not displayed
  // Draw the appropriate frame
  switch (currentFrame)
  {
  case 1:
    firstWXframe();
    break;
  case 2:
    secondWXframe();
    break;
  case 3:
    almanacFrame();
    break;
  case 4:
    tmrSecondTick.start(); // Restart second tick timer for clock updates
    if (DIGITAL_CLOCK)
    {
      digitalClockFrame(true); // Full draw on entry
    }
    else if (ANALOG_CLOCK)
    {
      analogClockFrame(true); // Full draw on entry
    }
    break;
  default:
    // Handle unexpected frame numbers, if needed
    break;
  }
}

/**
 * @brief Draws upper and lower panels for the frame.
 *
 * This function fills the top and bottom panels of the display with specified background colors.
 * It uses rounded rectangles for the top and bottom edges to create a visually appealing frame.
 *
 * @param top_background Color for the top panel background.
 * @param bottom_background Color for the bottom panel background.
 */
void drawFramePanels(int top_background, int bottom_background)
{
  // draws an upper and lower panel for the frame
  tft.fillRoundRect(0, 0, SCREEN_W, 2 * HEADER_RAD, HEADER_RAD, top_background);
  tft.fillRect(0, HEADER_RAD, SCREEN_W, HEADER_Y - HEADER_RAD, top_background);
  tft.fillRect(0, HEADER_Y, SCREEN_W, SCREEN_H - HEADER_Y - HEADER_RAD, bottom_background);
  tft.fillRoundRect(0, SCREEN_H - 2 * HEADER_RAD, SCREEN_W, 2 * HEADER_RAD, HEADER_RAD, bottom_background);
} // drawFramePanels()

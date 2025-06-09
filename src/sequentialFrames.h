/**
 * @file sequentialFrames.h
 * @author Karl Berger
 * @date 2025-06-04
 * @brief Sequential frames for the TFT display
 * @details This file contains the functions for displaying sequential frames
 *          on the TFT display. The frames include weather data, almanac data,
 *          and clock data.
 */

#ifndef SEQUENTIAL_FRAMES_H
#define SEQUENTIAL_FRAMES_H

void updateSequentialFrames();									 ///< update the sequential frames
void drawFramePanels(int top_background, int bottom_background); ///< draws upper and lower panels for the frame
void updateClock();												 ///< updates the selected clock

#endif															 // SEQUENTIAL_FRAMES_H
// End of file

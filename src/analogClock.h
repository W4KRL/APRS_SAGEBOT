/**
 * @file analogClock.h
 * @author Karl Berger
 * @date 2025-06-01
*/
#ifndef ANALOG_CLOCK_H
#define ANALOG_CLOCK_H

#include <Arduino.h>          // Arduino functions

/**
 * @brief Draws or clears the analog clock frame.
 *
 * This function is responsible for rendering the frame of an analog clock
 * on the display. If the parameter is set to true, the frame will be drawn;
 * if set to false, only the changing hand motions will be updated.
 *
 * @param drawFrame If true, draws the analog clock frame; 
 * if false, it updates only the second and minute hands.
 */
void analogClockFrame(bool drawFrame);

#endif // ANALOG_CLOCK_H
// End of file

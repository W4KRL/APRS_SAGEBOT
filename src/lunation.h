/**
 * @file lunation.h
 * @author Karl W. Berger
 * @date 2025-05-28
 * @brief Provides functions and structures for calculating and displaying the moon phase.
 *
 * This header defines the `phaseName` structure for representing the name of a moon phase,
 * and declares functions for determining the moon phase, retrieving its name, and drawing
 * a graphical representation of the moon's face.
 *
 * Structures:
 * - phaseName: Holds the two-word name of a moon phase.
 *
 * Functions:
 * - phaseName findPhaseName(float fraction): Returns the name of the moon phase based on the given fraction.
 * - void drawMoonFace(int xc, int yc, int r, float fract, int liteColor, int darkColor): Draws the moon face at the specified coordinates and radius, using the given phase fraction and colors.
 * - float moonPhase(): Calculates and returns the current moon phase as a fraction.
  */

#ifndef LUNATION_H
#define LUNATION_H

#include <Arduino.h>

struct phaseName
{ 
  String firstWord;
  String secondWord;
};

phaseName findPhaseName(float fraction);
void drawMoonFace(int xc, int yc, int r, float fract, int liteColor, int darkColor);
float moonPhase();

#endif // LUNATION_H

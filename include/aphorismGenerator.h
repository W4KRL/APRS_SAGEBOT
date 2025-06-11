/**
 * @file aphorismGenerator.h
 * @author Karl Berger
 * @date 2025-06-01
 * @brief Header file for aphorism generator functions and variables.
 *
 * @details This header declares functions and external variables used for generating and selecting
 * aphorisms from a designated file. The module supports mounting a filesystem, shuffling an array,
 * and selecting an aphorism from a file for display or further use.
 *
 * @section variables Variables
 * - **lineArray**: A pointer to an integer array used to store line indices for aphorism selection.
 * - **lineCount**: An integer representing the number of lines/aphorisms in the file.
 *
 * @section functions Functions
 * - **mountFS()**: Mounts the filesystem for access to aphorism files.
 * - **shuffleArray(int *array, int size)**: Shuffles the contents of an integer array to randomize selection order.
 * - **pickAphorism(String aphorismFile, int *lineArray)**: Selects and returns a random aphorism from the specified file
 *   using the shuffled line indices.
 */

#ifndef APHORISM_GENERATOR_H
#define APHORISM_GENERATOR_H

#include <Arduino.h> // for String class

extern int *lineArray;
extern int lineCount;

void mountFS();
void shuffleArray(int *array, int size);
String pickAphorism(String aphorismFile, int *lineArray);

#endif // APHORISM_GENERATOR_H
// End of file


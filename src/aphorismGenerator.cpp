/**
 * @file aphorismGenerator.cpp
 * @author Karl Berger
 * @date 2025-06-02
 * @brief Implementation of aphorism generator functions for the magloop controller project.
 *
 * This file provides functions to manage, shuffle, and retrieve aphorisms from a file stored on the device's filesystem.
 * It includes logic for mounting the filesystem, counting lines in the aphorism file, shuffling the order of aphorisms,
 * and selecting a random aphorism for display or use elsewhere in the application.
 *
 * Key Functions:
 * - mountFS(): Mounts the filesystem, counts aphorisms, and initializes a shuffled index array.
 * - shuffleArray(): Implements the Fisher-Yates shuffle to randomize the order of aphorisms.
 * - pickAphorism(): Retrieves an aphorism from the file based on the shuffled index array.
 *
 * Dependencies:
 * - LittleFS for file storage and access.
 * - credentials.h for configuration such as the aphorism file name.
 * - wug_debug.h for debug output.
 */

#include "aphorismGenerator.h"

#include <Arduino.h>     // Arduino functions
#include <LittleFS.h>    // [builtin]
#include "credentials.h" // Wi-Fi and weather station credentials
#include "wug_debug.h"   // for debug print

int *lineArray = nullptr; // holds shuffled index to aphorisms
int lineArraySize = 0;    // tracks the size of the lineArray

/**
 * @brief Mounts the LittleFS filesystem, counts the number of lines in the aphorism file,
 *        and initializes a shuffled array of line indices for random access.
 *
 * This function attempts to mount the LittleFS filesystem. If mounting fails, it logs an error and returns.
 * Upon successful mounting, it opens the aphorism file specified by APHORISM_FILE in read mode.
 * It then counts the number of lines in the file to determine how many aphorisms are available.
 * After counting, it initializes an array of integers (lineArray) with indices corresponding to each line,
 * and shuffles the array using a Fisher-Yates shuffle algorithm seeded with analog noise for better randomness.
 * This shuffled array can be used to access aphorisms in a random order without repetition.
 *
 * @note Assumes that APHORISM_FILE, lineArray, and lineArraySize are defined elsewhere.
 * @note Uses DEBUG_PRINT and DEBUG_PRINTLN macros for logging.
 * @note Requires LittleFS and random number generation to be available.
 */
void mountFS()
{
  if (!LittleFS.begin())
  {
    DEBUG_PRINTLN("FS error");
    return;
  }
  DEBUG_PRINTLN("FS mounted");

  File file = LittleFS.open(APHORISM_FILE, "r");
  if (!file)
  {
    DEBUG_PRINTLN("FS failed to open file");
  }
  /************** Count Lines in File *******************/
  int lineCount = 0;
  while (file.available())
  {
    file.readStringUntil('\n');
    lineCount++;
  }
  file.close();

  DEBUG_PRINT("FS: ");
  DEBUG_PRINT(lineCount);
  DEBUG_PRINTLN(" lines in " + APHORISM_FILE);

  // Create and shuffle the array directly during initialization
  lineArraySize = lineCount;
  lineArray = new int[lineArraySize];
  randomSeed(analogRead(0)); // Seed with analog noise from pin 0 for better entropy
  for (int i = 0; i < lineCount; i++)
  {
    int j = random(0, i + 1);                   // Generate a random index
    lineArray[i] = (j == i) ? i : lineArray[j]; // Place the new index or swap with an existing one
    lineArray[j] = i;
  }
} // mountFS()

/**
 * @brief Shuffles the elements of an integer array in place using the Fisher-Yates algorithm.
 *
 * This function randomly permutes the elements of the given array, ensuring each possible
 * permutation is equally likely. It iterates from the end of the array to the beginning,
 * swapping each element with another randomly selected element that comes before it (or itself).
 *
 * @param array Pointer to the integer array to be shuffled.
 * @param size The number of elements in the array.
 *
 * @note Uses the random() function to generate random indices. For improved randomness,
 *       consider seeding the random number generator with randomSeed() before calling this function.
 */
void shuffleArray(int *array, int size)
{
  // Fisher-Yates shuffle algorithm
  // This algorithm iterates through the array from the last element to the second.
  // For each element, it selects a random index (j) between 0 and the current index (i).
  // The elements at indices i and j are then swapped, ensuring a uniform shuffle.
  // random() is used to generate the random index, and it can be seeded with randomSeed() for better randomness.

  for (int i = size - 1; i > 0; i--)
  {
    int j = random(0, i + 1); // Generate a random index between 0 and i (inclusive)
    // Swap array[i] with the element at random index j
    int temp = array[i];
    array[i] = array[j];
    array[j] = temp;
  }
} // shuffleArray()

/**
 * @brief Picks an aphorism from a file based on a sequence of line numbers.
 *
 * This function retrieves a specific line (aphorism) from a file stored in the filesystem,
 * using the provided array of line numbers. It maintains an internal static index to cycle
 * through the line numbers on each call. If the end of the array is reached (indicated by -1),
 * the index resets to the beginning. The function attempts up to 10 times to read the desired
 * line from the file, returning an empty string on failure.
 *
 * @param fileName   The name of the file containing aphorisms, one per line.
 * @param lineArray  Pointer to an array of integers specifying the line numbers to pick.
 *                   The array should be terminated with -1.
 * @return           The aphorism string from the specified line, or an empty string on failure.
 */
String pickAphorism(String fileName, int *lineArray)
{
  static int j = 0; // Static variable to retain value between function calls

  if (lineArray == nullptr)
  {
    return ""; // Return an empty string if lineArray is not initialized
  }

  for (int attempt = 0; attempt < 10; attempt++)
  { // Limit attempts to prevent infinite loop
    File file = LittleFS.open(fileName.c_str(), "r");
    if (!file)
    {
      DEBUG_PRINTLN("FS failed to open file");
      return ""; // Return empty string on failure
    }

    String aphorism = "";
    int targetLine = lineArray[j];
    int currentLine = 0;

    // Search for the target line
    while (file.available())
    {
      String line = file.readStringUntil('\n');
      if (currentLine == targetLine)
      {
        aphorism = line;
        break;
      }
      currentLine++;
    }

    file.close(); // Ensure file is closed

    if (currentLine == targetLine)
    {
      j++; // Increment j for the next call
      if (lineArray[j] == -1)
      {        // Assuming the end of the array is marked with -1
        j = 0; // Reset j if it reaches the end of the array
      }
      return aphorism; // Return found aphorism
    }
  }

  j = 0;     // Reset j if no valid aphorism is found
  return ""; // Return empty string if unsuccessful
}

// End of file
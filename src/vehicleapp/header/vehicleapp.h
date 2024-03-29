/**
 * @file vehicleapp.h
 * @brief Vehicle Application header file
 *
 */

#include "../../vehicle/src/vehicle.cpp"

using namespace std;

/**
 * @brief ANSI escape code for red text color in terminal output.
 */
#define ANSI_COLOR_RED "\033[31m"

 /**
  * @brief ANSI escape code to reset text color in terminal output.
  */
#define ANSI_COLOR_RESET "\033[0m"

  /**
   * @brief Width of the terminal window used in the application.
   *        Adjusting this value can affect the layout of the application.
   */
int windowWidth = 21;

/**
 * @brief Height of the terminal window used in the application.
 *        Adjusting this value can affect the layout of the application.
 */
int windowHeight = 9;
/**
 * @file utility_test.cpp
 * @brief Unit tests for the Vehicle class and related functions.
 */

//#define ENABLE_UTILITY_TEST
#include "gtest/gtest.h"
#include "../../utility/header/commonTypes.h"
#include "../../utility/header/vehicleUtility.h"

#include <fstream>
#include <sys/stat.h>
#include <cstdio>
#ifdef _WIN32
  #include <direct.h>
#elif __linux__
  #include <unistd.h>
#endif

// TODO


/**
 * @brief The main function of the test program.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line argument strings.
 * @return int The exit status of the program.
 */
int main(int argc, char **argv) {
#ifdef ENABLE_UTILITY_TEST
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
#else
  return 0;
#endif
}

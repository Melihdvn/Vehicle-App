/**
 * @file commonTypes.h
 * 
 * @brief Provides cross-platform type definitions
 */

#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <stdexcept> // Include for standard exception classes

#if defined(_WIN32)
#include <cstddef>
#include <cstdint>    ///< Include for standard integer types on Windows
#include <inttypes.h> ///< Include for integer format macros on Windows
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <iomanip>
#include <queue>
#define CLEAR_SCREEN "cls"
#elif defined(__linux__)
#include <cstddef>
#include <cstdint>
#include <stdint.h>   ///< Include for standard integer types on Linux
#include <inttypes.h> ///< Include for integer format macros on Linux
#include "../../utility/header/commonTypes.h"
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <iomanip>
#include <queue>
#include <termios.h>
#include <unistd.h>
#define CLEAR_SCREEN "clear"

char getch() {
    char buf = 0;
    struct termios old = { 0 };
    fflush(stdout);

    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");

    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;

    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");

    if (read(0, &buf, 1) < 0)
        perror("read()");

    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;

    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");

    return buf;
}

#elif defined(__APPLE__)
#include <cstddef>
#include <cstdint>
#include <stdint.h>   ///< Include for standard integer types on Apple platforms
#include <inttypes.h> ///< Include for integer format macros on Apple platforms
#endif

#endif // COMMON_TYPES_H

/*
This is the header file for the Calendar tool
*/

#include "led-matrix.h"
#include "graphics.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <iostream>
#include <filesystem>
#include <ctime>
#include <chrono>
#include <linux/limits.h>
#include <libgen.h>
using namespace rgb_matrix;

// Constants
constexpr int boardDimension    { 64 };
constexpr int letter_spacing    { 0  };
constexpr int x_origin          { 2  }; // Where text can begin appearing on the board in the x direction (it will not be on the very edge of the board)
          int y_origin          { 2  }; // Where text can begin appearing on the board in the y direction (it by default appears with a one pixel gap at the top, so 0 is astute here)

char result[PATH_MAX];
ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);

// ***** Interactable Stuff *****

// >> Color of the text <<
Color color(231, 84, 128);      // hot pink

// >> Font Size <<
std::string path = std::string(dirname(result)) + "/font.bdf";
const char* chosenFont = path.data();



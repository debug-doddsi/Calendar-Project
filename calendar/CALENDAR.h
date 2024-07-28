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

#include <ctime>
#include <chrono>
using namespace rgb_matrix;

// Constants
constexpr int boardDimension    { 64 };
constexpr int letter_spacing    { 0  };
constexpr int x_origin          { 1  }; // Where text can begin appearing on the board in the x direction (it will not be on the very edge of the board)
          int y_origin          { 5  }; // Where text can begin appearing on the board in the y direction (it by default appears with a one pixel gap at the top, so 0 is astute here)


// ***** Interactable Stuff *****

// >> Color of the text <<
Color color(231, 84, 128);      // hot pink


// >> Font Size <<
const char* chosenFont = "";




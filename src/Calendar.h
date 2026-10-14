/*
This is the header file for the Calendar tool
*/

#pragma once

#include "led-matrix.h"
#include "graphics.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ads1115.h"

#include <iostream>
#include <filesystem>
#include <ctime>
#include <chrono>
using namespace rgb_matrix;

// Constants
inline constexpr int boardDimension    { 64 };
inline constexpr int letter_spacing    { 0  };
inline constexpr int x_origin          { 2  }; // Where text can begin appearing on the board in the x direction (it will not be on the very edge of the board)
inline int y_origin          { 2  }; // Where text can begin appearing on the board in the y direction (it by default appears with a one pixel gap at the top, so 0 is astute here)

// Directory containing the running executable
inline std::filesystem::path exeDir()
{
  return std::filesystem::read_symlink("/proc/self/exe").parent_path();
}

// ***** Interactable Stuff *****

// >> Color of the text <<
inline Color color(231, 84, 128);      // hot pink

// >> Font Size <<
inline const std::string path = (exeDir() / ".." / "font.bdf").string();
inline const char* chosenFont = path.c_str();

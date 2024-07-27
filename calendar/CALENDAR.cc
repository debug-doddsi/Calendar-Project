// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
// Small example how write text.
//
// This code is public domain
// (but note, that the led-matrix library this depends on is GPL v2)

#include "led-matrix.h"
#include "graphics.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

using namespace rgb_matrix;

constexpr int boarddimensions{64};

int main(int argc, char *argv[])
{
  RGBMatrix::Options matrix_options;
  rgb_matrix::RuntimeOptions runtime_opt;

  matrix_options.cols = boarddimensions;
  matrix_options.rows = boarddimensions;
  matrix_options.disable_hardware_pulsing = true;

  Color color(231, 84, 128); // pink!!

  int x_orig{0};
  int y_orig{0};
  int letter_spacing{0};

  // Load font. This needs to be a filename with a bdf bitmap font.
  rgb_matrix::Font font;
  if (!font.LoadFont("../fonts/helvR12.bdf"))
  {
    fprintf(stderr, "Couldn't load font '%s'\n", "helvR12.bdf");
    return 1;
  }

  RGBMatrix *canvas = RGBMatrix::CreateFromOptions(matrix_options, runtime_opt);
  if (canvas == NULL)
    return 1;

  const int x = x_orig;
  int y = y_orig;

  char* text[] = {"hello",
                  "you",
                  "cunt"};

for(auto i:text)
{
  rgb_matrix::DrawText(canvas, font, x, y + font.baseline(),
                       color, NULL, i,
                       letter_spacing);

      y += font.height();
};

  char line[1024];
  fgets(line, sizeof(line), stdin);
  // Finished. Shut down the RGB matrix.
  delete canvas;

  return 0;
}

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

int main(int argc, char *argv[])
{
  RGBMatrix::Options matrix_options;
  rgb_matrix::RuntimeOptions runtime_opt;
  rgb_matrix::ParseOptionsFromFlags(&argc,
                                    &argv,
                                    &matrix_options,
                                    &runtime_opt);

  Color color(231, 84, 128); // pink!!
  Color bg_color(0, 0, 0);

  int x_orig = 0;
  int y_orig = 0;
  int letter_spacing = 0;

  /*
   * Load font. This needs to be a filename with a bdf bitmap font.
   */
  rgb_matrix::Font font;
  font.LoadFont("../fonts/helvR12.bdf");

  /*
   * If we want an outline around the font, we create a new font with
   * the original font as a template that is just an outline font.
   */
  rgb_matrix::Font *outline_font = NULL;

  RGBMatrix *canvas = RGBMatrix::CreateFromOptions(matrix_options, runtime_opt);
  if (canvas == NULL)
    return 1;

  const int x = x_orig;
  int y = y_orig;

  rgb_matrix::DrawText(canvas, font, x, y + font.baseline(),
                       color, outline_font ? NULL : &bg_color, "hi",
                       letter_spacing);

  char line[1024];
  fgets(line, sizeof(line), stdin);
  // Finished. Shut down the RGB matrix.
  delete canvas;

  return 0;
}

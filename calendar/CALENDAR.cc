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

constexpr int boarddimensions{64};

int main(int argc, char *argv[])
{
  RGBMatrix::Options matrix_options;
  rgb_matrix::RuntimeOptions runtime_opt;

  matrix_options.cols = boarddimensions;
  matrix_options.rows = boarddimensions;
  matrix_options.disable_hardware_pulsing = true;

  Color color(231, 84, 128); // pink!!

  int letter_spacing{0};

  // Load font. This needs to be a filename with a bdf bitmap font.
  rgb_matrix::Font font;
  if (!font.LoadFont("../fonts/5x8.bdf"))
  {
    fprintf(stderr, "Couldn't load font '%s'\n", "helvR12.bdf");
    return 1;
  }

  RGBMatrix *canvas = RGBMatrix::CreateFromOptions(matrix_options, runtime_opt);
  if (canvas == NULL)
    return 1;

  const int x = 1;
  int y = 0;

std::time_t date = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
struct tm datetime = *localtime(&date);
char buffer [50];
strftime(buffer, 50, "%a %e %b", &datetime);

char* text[] = {buffer};

//font.CharacterWidth
for(auto i:text)
{
  rgb_matrix::DrawText(canvas,
                       font, 
                       x, 
                       y + font.baseline(),
                       color, 
                       NULL, 
                       i,
                       letter_spacing);

      y += font.height();
};

  char line[1024];
  fgets(line, sizeof(line), stdin);
  
  // Finished. Shut down the RGB matrix.
  delete canvas;

  return 0;
}

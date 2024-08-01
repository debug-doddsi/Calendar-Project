/*
This is the main file for the Calendar tool
*/

#include "CALENDAR.h"

using namespace rgb_matrix;

int main(int argc, char *argv[])
{
  RGBMatrix::Options          matrix_options;
  rgb_matrix::RuntimeOptions  runtime_opt;

  matrix_options.cols                     = boardDimension;
  matrix_options.rows                     = boardDimension;
  matrix_options.disable_hardware_pulsing = true; // Needs to be set true to get around an incompatibility with the board

  // Load font- This needs to be a filename with a bdf bitmap font.
  rgb_matrix::Font font;

  fprintf(stdout, "Font path: %s\n\n", chosenFont);

  if (!font.LoadFont(chosenFont))
  {
    fprintf(stderr, "Couldn't load font\n\n");
    return 1;
  }

RGBMatrix *canvas = RGBMatrix::CreateFromOptions(matrix_options, runtime_opt);

std::time_t date = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
struct tm datetime = *localtime(&date);
char todaysDate [50]; // Allocate this amount of memory
strftime(todaysDate, 50, "%a %e %b", &datetime);

const char* text[] = {todaysDate,
                      "",                 // Empty line
                      "> Breakfast",      // Sample entry 1
                      "  @ 9am",
                      "> Lunch",          // Sample entry 2
                      "  @ 12pm",
                      "> Dinner",         // Sample entry 3
                      "  @ 6pm"
                      };



// This for loop operates like a typewriter
for(auto i:text)
{
  // Print along the x axis
  rgb_matrix::DrawText(canvas, // knows it is a 64x64 matrix
                       font, 
                       x_origin, 
                       y_origin + font.baseline(),
                       color, 
                       NULL, 
                       i,
                       letter_spacing);

  // Move down the y axis onto a new line
  y_origin += font.height();
};

  sleep(10);
  
  // Finished. Shut down the RGB matrix.
  delete canvas;

  return 0;
}

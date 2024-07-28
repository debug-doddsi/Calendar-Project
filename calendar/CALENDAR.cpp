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
  if (!font.LoadFont(chosenFont))
  {
    fprintf(stderr, "Couldn't load font");
    return 1;
  }

  RGBMatrix *canvas = RGBMatrix::CreateFromOptions(matrix_options, runtime_opt);
  if (canvas == NULL)
  {
    return 1;
  }

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

//font.CharacterWidth
for(auto i:text)
{
  rgb_matrix::DrawText(canvas,
                       font, 
                       x_origin, 
                       y_origin + font.baseline(),
                       color, 
                       NULL, 
                       i,
                       letter_spacing);
  y_origin += font.height();
};

  char line[1024];
  fgets(line, sizeof(line), stdin);
  
  // Finished. Shut down the RGB matrix.
  delete canvas;

  return 0;
}

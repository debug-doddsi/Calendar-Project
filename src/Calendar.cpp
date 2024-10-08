/*
This is the main file for the Calendar tool
*/

#include "Calendar.h"
#include <pybind11/embed.h> // for embedding
#include <pybind11/eval.h>
#include <chrono>
#include <thread>

namespace py = pybind11;

using namespace rgb_matrix;

int main(int argc, char *argv[])
{
  
  // py::scoped_interpreter guard{}; // start the interpreter and keep it alive
  // py::object scope = py::module_::import("__main__").attr("__dict__");
  // py::eval_file("../Apple_API.py", scope);
  
  // RGBMatrix::Options matrix_options;
  // rgb_matrix::RuntimeOptions runtime_opt;

  // matrix_options.cols = boardDimension;
  // matrix_options.rows = boardDimension;
  // matrix_options.disable_hardware_pulsing = true; // Needs to be set true to get around an incompatibility with the board

  // // Load font- This needs to be a filename with a bdf bitmap font.
  // rgb_matrix::Font font;

  // fprintf(stdout, "Font path: %s\n\n", chosenFont);

  // if (!font.LoadFont(chosenFont))
  // {
  //   fprintf(stderr, "Couldn't load font\n\n");
  //   return 1;
  // }

  // RGBMatrix *canvas = RGBMatrix::CreateFromOptions(matrix_options, runtime_opt);

  // std::time_t date = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  // struct tm datetime = *localtime(&date);
  // char todaysDate[50]; // Allocate this amount of memory
  // strftime(todaysDate, 50, "%a %e %b", &datetime);

  // const char *events[] = {
  //     "> Breakfast", // Sample entry 1
  //     "  @ 9am",
  //     "> Lunch", // Sample entry 2
  //     "  @ 12pm",
  //     "> Dinner", // Sample entry 3
  //     "  @ 6pm"};

  // rgb_matrix::DrawText(canvas, // knows it is a 64x64 matrix
  //                      font,
  //                      x_origin,
  //                      y_origin + font.baseline(),
  //                      color,
  //                      NULL,
  //                      todaysDate,
  //                      letter_spacing);

  // y_origin += (font.height()) * 1.5; // new line plus a half (minimal gap between date and beginning of events)

  // // This for loop operates like a typewriter
  // for (auto i : events)
  // {
  //   // Print along the x axis
  //   rgb_matrix::DrawText(canvas, // knows it is a 64x64 matrix
  //                        font,
  //                        x_origin,
  //                        y_origin + font.baseline(),
  //                        color,
  //                        NULL,
  //                        i,
  //                        letter_spacing);

  //   // Move down the y axis onto a new line
  //   y_origin += font.height();
  // };

  // sleep(10);

  // // Finished. Shut down the RGB matrix.
  // delete canvas;

  ADS1115* pADC = new ADS1115("/dev/i2c-1", 0x48);
  
  while(true){
    double Y_value = pADC->ReadData(1);
    double X_value = pADC->ReadData(2);
    std::string message;

    if ( Y_value > 20000 )
    {
      message = "North";
    }
    else if ( Y_value < 6000 )
    {
      message = "South";
    }
    else if ( X_value > 20000 )
    {
      message = "East";
    }
    else if ( X_value < 6000 )
    {
      message = "WestSIDE MOTHERFUCKER";
    }
    else // shits fucked
    {
      message = "Center";
    }
        

    std::cout << "Joystick Direction: " << message << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  };

  return 0;
}

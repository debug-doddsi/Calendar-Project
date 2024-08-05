# Digital Calendar

- This project is intended to display my personal calendar on an LED matrix
- The default view will be of the current day's events
- There will be a joystick hidden in the decorative frame, integrated into the Pi with an ADC
  - DOWN: Show the rest of the events of the day (if there are too many to be viewed on the screen)
  - RIGHT: Show the next day
  - LEFT: Show the previous day
  - UP: Go back up to the top of the day's event list
- After 2 minutes of no joystick activity, the calendar will return to the top of the current day's event list
- The calendar shall poll the API every 30 minutes for changes to the calendar

## Building

    mkdir src/build
    cd src/build
    cmake ../
    make
    ./Calendar

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

    make                # builds the LED matrix library in lib/
    mkdir src/build
    cd src/build
    cmake ../
    make
    ./Calendar

## Google Calendar setup

Events come from the Google Calendar API (`gcal/`). Python, standard library only.

One-off setup, on your laptop:

1. In the [Google Cloud console](https://console.cloud.google.com/) create a project and enable the **Google Calendar API**.
2. Under **APIs & Services > OAuth consent screen**, set it up as External, add yourself as a test user, then **publish the app to production**. If you leave it in Testing, the refresh token expires after 7 days. You will see an "unverified app" warning when you sign in, which is fine for your own account.
3. Under **Credentials**, create an **OAuth client ID** of type **Desktop app** and download the JSON.
4. Run the login script:

       python3 gcal/auth_setup.py ~/Downloads/client_secret_xxx.json

   This writes `gcal/secrets.json` (gitignored) with your refresh token. Copy that file to the display device. Never commit it.
5. Check it works:

       python3 gcal/show_today.py

Tests (no network or credentials needed):

    cd gcal && python3 -m unittest discover -s tests -v

## C++ tests

The joystick and navigation logic in `src/Navigation.h` has no hardware dependencies and can be tested on any machine:

    cmake -S tests -B tests/build
    cmake --build tests/build
    ctest --test-dir tests/build

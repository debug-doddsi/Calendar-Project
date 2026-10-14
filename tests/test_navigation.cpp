#include "Navigation.h"

#include <cstdlib>
#include <iostream>

#define CHECK(cond)                                                        \
  do {                                                                     \
    if (!(cond)) {                                                         \
      std::cerr << __FILE__ << ":" << __LINE__ << " failed: " #cond "\n"; \
      std::exit(1);                                                        \
    }                                                                      \
  } while (0)

using namespace std::chrono;

int main()
{
  // classify
  CHECK(classify(16000, 16000) == Direction::Center);
  CHECK(classify(16000, 25000) == Direction::North);
  CHECK(classify(16000, 1000)  == Direction::South);
  CHECK(classify(25000, 16000) == Direction::East);
  CHECK(classify(1000, 16000)  == Direction::West);

  // navigation
  DayView::Clock::time_point t0 = DayView::Clock::now();
  DayView view;

  view.handle(Direction::South, t0);
  view.handle(Direction::South, t0);
  CHECK(view.scroll() == 2);

  view.handle(Direction::North, t0);
  CHECK(view.scroll() == 0);

  view.handle(Direction::South, t0);
  view.handle(Direction::East, t0);
  CHECK(view.dayOffset() == 1 && view.scroll() == 0);

  view.handle(Direction::West, t0);
  view.handle(Direction::West, t0);
  CHECK(view.dayOffset() == -1);

  view.handle(Direction::Center, t0);
  CHECK(view.dayOffset() == -1);

  // idle reset
  view.update(t0 + seconds(119));
  CHECK(view.dayOffset() == -1);
  view.update(t0 + seconds(120));
  CHECK(view.dayOffset() == 0 && view.scroll() == 0);

  std::cout << "All tests passed\n";
  return 0;
}

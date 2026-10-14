/*
Hardware-independent navigation logic, so it can be unit tested without a Pi.
*/

#pragma once

#include <chrono>

enum class Direction { Center, North, South, East, West };

// Raw ADS1115 readings are 16 bit signed, so full scale is 32767
inline constexpr double highThreshold { 20000 };
inline constexpr double lowThreshold  { 6000  };

inline Direction classify(double xValue, double yValue)
{
  if (yValue > highThreshold) return Direction::North;
  if (yValue < lowThreshold)  return Direction::South;
  if (xValue > highThreshold) return Direction::East;
  if (xValue < lowThreshold)  return Direction::West;
  return Direction::Center;
}

// Tracks which day is shown and how far down its event list the view is scrolled
class DayView
{
public:
  using Clock = std::chrono::steady_clock;

  explicit DayView(std::chrono::seconds idleTimeout = std::chrono::minutes(2))
      : idleTimeout_(idleTimeout) {}

  // DOWN scrolls, UP returns to top, RIGHT is next day, LEFT is previous day
  void handle(Direction direction, Clock::time_point now)
  {
    if (direction == Direction::Center) return;

    lastActivity_ = now;
    switch (direction)
    {
      case Direction::South: ++scroll_;                   break;
      case Direction::North: scroll_ = 0;                 break;
      case Direction::East:  ++dayOffset_; scroll_ = 0;   break;
      case Direction::West:  --dayOffset_; scroll_ = 0;   break;
      default: break;
    }
  }

  // After the idle timeout, go back to the top of today's list
  void update(Clock::time_point now)
  {
    if ((dayOffset_ != 0 || scroll_ != 0) && now - lastActivity_ >= idleTimeout_)
    {
      dayOffset_ = 0;
      scroll_    = 0;
    }
  }

  int dayOffset() const { return dayOffset_; }
  int scroll()    const { return scroll_;    }

private:
  std::chrono::seconds idleTimeout_;
  Clock::time_point lastActivity_ {};
  int dayOffset_ { 0 };
  int scroll_    { 0 };
};

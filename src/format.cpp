#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long total_seconds) {
  long hours = total_seconds / (60 * 60);
  long remaining_seconds = total_seconds - hours * 60 * 60;
  long minutes = remaining_seconds / 60;
  remaining_seconds -= minutes * 60;
  char s[10];
  sprintf(s, "%02ld:%02ld:%02ld", hours, minutes, remaining_seconds);
  return string(s);
}

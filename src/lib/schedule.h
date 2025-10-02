#pragma once
#include <array>
#include "globals.h"
#include "person.h"
#include "bathroom.h"

namespace scheduleAPI {
struct Day {
    const int minutesInDay = {120};
    std::array<bathroomAPI::Bathroom, 120> minutes;
};

struct Week {
    const int daysInWeek = {5}; // 5 School Days
    std::array<Day, 5> days;
};
} // namespace scheduleAPI
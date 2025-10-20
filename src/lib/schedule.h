#pragma once
#include "common_includes.h"
#include "person.h"
#include "bathroom.h"

namespace scheduleAPI {
struct Day {
    const int minutesInDay = {150};
    std::array<bathroomAPI::Bathroom, 150> minutes;
};

struct Week {
    const int daysInWeek = {5}; // 5 School Days
    std::array<Day, 5> days;
};
} // namespace scheduleAPI
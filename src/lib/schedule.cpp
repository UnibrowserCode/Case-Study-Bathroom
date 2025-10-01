#include "globals.h"
#include "person.h"
#include "bathroom.h"

namespace scheduleAPI {
struct Day {
    const int minutesInDay = {120};
    bathroomAPI::Bathroom minutes[120];
};

struct Week {
    const int daysInWeek = {5}; // 5 School Days
    Day days[7];
};
} // namespace scheduleAPI
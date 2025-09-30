#include "globals.h"
#include "person.h"
#include "bathroom.h"

namespace scheduleAPI {
struct Day {
    const int minutesInDay = {120};
    bathroom::Bathroom minutes[120];
};

struct Week {
    const int daysInWeek = {120};
    Day days[7];
};
}
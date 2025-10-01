#pragma once
#include "globals.h"
#include <vector>
#include <array>

namespace bathroomAPI {
    enum class BathroomStation : uint8_t;
}

namespace personAPI {

struct Task {
    bathroomAPI::BathroomStation station;
    int timeRequired;
    bool completed;
};

// Person enum
enum class PersonName : uint8_t {
    None    = 0,
    Mom     = 1 << 0,
    Dad     = 1 << 1,
    Heather = 1 << 2,
    Nick    = 1 << 3,
    Rulon   = 1 << 4,
    George  = 1 << 5,
    Olivia  = 1 << 6,
    Thomas  = 1 << 7
};

struct Person {
    PersonName name;
    bathroomAPI::BathroomStation curStation;
    int timeLeftUsing;
    std::array<Task, 2> tasks;
};

} // namespace personAPI
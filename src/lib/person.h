#pragma once
#include "globals.h"
#include "bathroom.h"

using namespace bathroom;

namespace personAPI {

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
    bathroom::BathroomStation curStation;
};

} // namespace person_api
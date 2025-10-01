#pragma once
#include "globals.h"
#include "bathroom.h"
#include "person.h"
#include <vector>

using namespace bathroomAPI;

namespace personAPI {

struct Task {
    BathroomStation station;
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
    BathroomStation curStation;
    int timeLeftUsing;
    vector<Task> tasks;
};

inline personAPI::PersonName operator|(personAPI::PersonName a, personAPI::PersonName b) {
    return static_cast<personAPI::PersonName>(
        static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

inline personAPI::PersonName operator&(personAPI::PersonName a, personAPI::PersonName b) {
    return static_cast<personAPI::PersonName>(
        static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}

inline personAPI::PersonName& operator|=(personAPI::PersonName& a, personAPI::PersonName b) {
    return a = a | b;
}

} // namespace person_api
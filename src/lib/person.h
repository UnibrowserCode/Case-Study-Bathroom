#pragma once
#include <array>
#include "globals.h"

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
    unsigned int timeLeftUsing;
    std::array<Task, 2> tasks;
    inline bool allTasksCompleted() {
        return tasks[0].completed && tasks[1].completed;
    }
};

// Option 2: to_string (use std::string s = to_string(name);)
inline std::string to_string(PersonName name) {
    switch (name) {
        case PersonName::None:    return "None";
        case PersonName::Mom:     return "Mom";
        case PersonName::Dad:     return "Dad";
        case PersonName::Heather: return "Heather";
        case PersonName::Nick:    return "Nick";
        case PersonName::Rulon:   return "Rulon";
        case PersonName::George:  return "George";
        case PersonName::Olivia:  return "Olivia";
        case PersonName::Thomas:  return "Thomas";
        default:                  return "Unknown";
    }
}

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

} // namespace personAPI
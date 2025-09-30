#pragma once
#include "globals.h"
#include "bathroom.h"
#include "person.h"

using namespace bathroom;

namespace personAPI {

// Person enum
const enum class PersonName : uint8_t {
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

class Person {
    private:
    public:
        PersonName name;
        BathroomStation curStation;
        Person(PersonName name_, BathroomStation curStation_) {
            name = name_;
            curStation = curStation_;
        }
};

inline person::PersonName operator|(person::PersonName a, person::PersonName b) {
    return static_cast<person::PersonName>(
        static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

inline person::PersonName operator&(person::PersonName a, person::PersonName b) {
    return static_cast<person::PersonName>(
        static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}

inline person::PersonName& operator|=(person::PersonName& a, person::PersonName b) {
    return a = a | b;
}

} // namespace person_api
#pragma once
#include "globals.h"
#include "person.h"

using namespace personAPI;

namespace bathroomAPI {

// Forward declarations
enum class PersonName : uint8_t;
enum class BathroomStation : uint8_t;
class Schedule;

// BathroomStation enum
enum class BathroomStation : uint8_t {
    None   = 0,
    Sink1  = 1 << 0,
    Sink2  = 1 << 1,
    Tub    = 1 << 2,
    Shower = 1 << 3
};

struct Occupation {
    Person people[8] = {};
};

// Bathroom class declaration
class Bathroom {
public:  // <--- stations and occupants public
    uint8_t stations;
    uint8_t occupants;
    Occupation occupation = {};
public:
    Bathroom(uint8_t stations_ = 0, uint8_t occupants_ = 0, Occupation occupation_ = {});
    inline bool isFree(BathroomStation station) const;
    void takeStation(BathroomStation station, PersonName user);
    void releaseStation(BathroomStation station, PersonName user);
    inline bool stationAvailable(BathroomStation station, PersonName user) const;
    inline bool isUsingStation(PersonName user) const;
};

// Operator overloads (inline here for convenience)
inline BathroomStation operator|(BathroomStation a, BathroomStation b) {
    return static_cast<BathroomStation>(
        static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

inline BathroomStation operator&(BathroomStation a, BathroomStation b) {
    return static_cast<BathroomStation>(
        static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}

inline BathroomStation& operator|=(BathroomStation& a, BathroomStation b) {
    return a = a | b;
}

} // namespace bathroom_api

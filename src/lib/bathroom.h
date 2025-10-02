#pragma once
#include <array>
#include "globals.h"
#include "person.h"

namespace bathroomAPI {

enum class BathroomStation : uint8_t {
    None   = 0,
    Sink1  = 1 << 0,
    Sink2  = 1 << 1,
    Tub    = 1 << 2,
    Shower = 1 << 3
};

class Bathroom {
public:
    uint8_t stations;
    uint8_t occupants;
    std::array<personAPI::Person, 8> occupation = {};
public:
    Bathroom(uint8_t stations_ = 0, uint8_t occupants_ = 0, std::array<personAPI::Person, 8> occupation_ = {})
        : stations(stations_), occupants(occupants_), occupation(occupation_) {}

    // Check if a station is free (no one is using it)
    inline bool isFree(BathroomStation station) const {
        for (const auto& person : occupation) {
            if ((person).curStation == station)
                return false;
        }
        return true;
    }

    // Check if a station is available for a specific user
    inline bool stationAvailable(BathroomStation station, personAPI::PersonName user) const {
        for (const auto& person : occupation) {
            if ((person).curStation == station && (person).name != user)
                return false;
        }
        return true;
    }

    // Check if a specific user is using any station
    inline bool isUsingStation(personAPI::PersonName user) const {
        for (const auto& person : occupation) {
            if ((person).name == user && (person).curStation != BathroomStation::None)
                return true;
        }
        return false;
    }

    void takeStation(BathroomStation station, personAPI::PersonName user) {
        for (auto& person : occupation) {
            if ((person).name == user) {
                (person).curStation = station;
                break;
            }
        }
    }

    void releaseStation(BathroomStation station, personAPI::PersonName user) {
        for (auto& person : occupation) {
            if ((person).name == user && (person).curStation == station) {
                (person).curStation = BathroomStation::None;
                break;
            }
        }
    }
};

// Option 2: to_string (use std::string s = to_string(name);)
inline std::string to_string(BathroomStation station) {
    switch (station) {
        case BathroomStation::None:   return "None";
        case BathroomStation::Shower: return "Shower";
        case BathroomStation::Sink1:  return "Sink1";
        case BathroomStation::Sink2:  return "Sink2";
        case BathroomStation::Tub:    return "Bathtub";
        default:                      return "Unknown";
    }
}

// Operator overloads
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

} // namespace bathroomAPI

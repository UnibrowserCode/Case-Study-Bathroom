#pragma once
#include <array>
#include "globals.h"
#include "person.h"
#include <iostream>

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
        inline bool isTaken(uint8_t &station) const {
            return station & stations;
        }
        void takeStation(BathroomStation &station, personAPI::PersonName &user) {
            stations |= static_cast<uint8_t>(station);
            occupants |= static_cast<uint8_t>(user);
        }
        void releaseStation(BathroomStation &station, personAPI::PersonName &user) {
            stations &= ~static_cast<uint8_t>(station);
            occupants &= ~static_cast<uint8_t>(user);
        }
        inline bool isUsingStation(uint8_t &user) const {
            return occupants & user;
        }
        /*
        * Returns whether a station is available based on a set of rules.
        * 
        * RULES:
        * - Both sinks can be in use at the same time
        * - If someone is in the shower nobody else can be in the bathroom*
        *
        * EXCEPTIONS:
        * - Mom and Dad can be in the bathroom together even while one is showering
        * - If Thomas is bathing, others can use the sinks
        */
        inline bool stationAvailable(BathroomStation &station, personAPI::PersonName &user) {
            constexpr uint8_t momDadMask = static_cast<uint8_t>(personAPI::PersonName::Mom) | static_cast<uint8_t>(personAPI::PersonName::Dad);
            uint8_t userUint = static_cast<uint8_t>(user);
            uint8_t stationUint = static_cast<uint8_t>(station);
            if (isTaken(stationUint)) return false;
            if (isUsingStation(userUint)) return false;
            if (!stations) return true;
            // Mom + Dad = 3, everyone else's baseline values are above 3.
            // If only mom and/or dad are in the bathroom, everything is open.
            if (userUint & momDadMask && !(occupants & ~momDadMask)) return true;
            return !(stations & static_cast<uint8_t>(BathroomStation::Shower)) && !(station == BathroomStation::Shower);
        }
};
// Option 2: to_string (use std::string s = to_string(name);)
inline std::string to_string(bathroomAPI::BathroomStation station) {
    switch (station) {
        case BathroomStation::None:   return "None";
        case BathroomStation::Shower: return "Shower";
        case BathroomStation::Sink1:  return "Sink1";
        case BathroomStation::Sink2:  return "Sink2";
        case BathroomStation::Tub:    return "Bathtub";
        default:                      return "Unknown";
    }
}
} // namespace bathroomAPI
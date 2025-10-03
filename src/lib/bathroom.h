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
        inline bool isTaken(BathroomStation &station) const {
            return static_cast<uint8_t>(station) & stations;
        }
        void takeStation(BathroomStation &station, personAPI::PersonName &user) {
            stations |= static_cast<uint8_t>(station);
            occupants |= static_cast<uint8_t>(user);
        }
        void releaseStation(BathroomStation &station, personAPI::PersonName &user) {
            stations &= ~static_cast<uint8_t>(station);
            occupants &= ~static_cast<uint8_t>(user);
        }
        inline bool isUsingStation(personAPI::PersonName &user) const {
            return occupants & static_cast<uint8_t>(user);
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
            if (isTaken(station)) return false;
            if (isUsingStation(user)) return false;
            if (!stations) return true;
            // Mom + Dad = 3, everyone else's baseline values are above 3.
            // If only mom and/or dad are in the bathroom, everything is open.
            if ((static_cast<uint8_t>(user) & (static_cast<uint8_t>(personAPI::PersonName::Mom) | static_cast<uint8_t>(personAPI::PersonName::Dad))) != 0 && (occupants & ~static_cast<uint8_t>(personAPI::PersonName::Mom | personAPI::PersonName::Dad)) == 0) {
                return true;
            }
            return !(stations & static_cast<uint8_t>(BathroomStation::Shower)); // Shower is the cutoff value. It should be the highest value of all the bathroom stations
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
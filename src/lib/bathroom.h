#pragma once
#include <iostream>
#include "common_includes.h"
#include "global_enums.h"
#include "person.h"


namespace bathroomAPI {


class Bathroom {
public:
    uint8_t stations;
    uint8_t occupants;
    personAPI::Family occupation = {};
public:
    Bathroom(uint8_t stations_ = 0, uint8_t occupants_ = 0, personAPI::Family occupation_ = {})
        : stations(stations_), occupants(occupants_), occupation(occupation_) {}

        // Check if a station is free (no one is using it)
        inline bool isTaken(BathroomStation &station) const {
            return station & stations;
        }
        inline void takeStation(BathroomStation &station, personAPI::PersonName &name, int personIdx) {
            stations |= station;
            occupants |= name;
            occupation.members.at(personIdx).takeStation(station);
        }
        inline void releaseStation(BathroomStation &station, personAPI::PersonName &name, int personIdx) {
            stations &= ~station;
            occupants &= ~name;
            occupation.members.at(personIdx).releaseStation();
        }
        inline bool isUsingStation(personAPI::PersonName &name) const {
            return occupants & name;
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
            if (isTaken(station)) return false;
            if (isUsingStation(user)) return false;
            if (!stations) return true;
            // If only mom and/or dad are in the bathroom, everything is open.
            if (user & momDadMask && !(occupants & ~momDadMask)) return true;
            return !(stations & BathroomStation::Shower) && !(station == BathroomStation::Shower);
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
        case BathroomStation::Meeting:return "Meeting";
        default:                      return "Unknown";
    }
}
} // namespace bathroomAPI
#include "bathroom.h"
#include "person.h"

namespace bathroomAPI {

// Bathroom constructor
Bathroom::Bathroom(uint8_t stations_, uint8_t occupants_, Occupation occupation_) {
    stations = stations_;
    occupants = occupants_;
    occupation = occupation_;
}

// Check if a station is free
inline bool Bathroom::isFree(BathroomStation station) const {
    return !(static_cast<uint8_t>(station) & stations);
}

// Take a station
void Bathroom::takeStation(BathroomStation station, PersonName user) {
    stations |= static_cast<uint8_t>(station);
    occupants |= static_cast<uint8_t>(user);
}

// Release a station
void Bathroom::releaseStation(BathroomStation station, PersonName user) {
    stations &= ~static_cast<uint8_t>(station);
    occupants &= ~static_cast<uint8_t>(user);
}

inline bool Bathroom::isUsingStation(PersonName user) const {
    return occupants & static_cast<uint8_t>(user);
}

// Check availability of a station according to your rules
inline bool Bathroom::stationAvailable(BathroomStation station, PersonName user) const {
    constexpr uint8_t momDadMask = static_cast<uint8_t>(personAPI::PersonName::Mom) | static_cast<uint8_t>(personAPI::PersonName::Dad);
    if (!isFree(station) || isUsingStation(user)) return false;
    if (!stations) return true;
    // Mom + Dad logic
    if ((static_cast<uint8_t>(user) & momDadMask) && !(occupants & ~momDadMask)) return true;
    // Shower logic
    if ((static_cast<uint8_t>(station) & ~static_cast<uint8_t>(BathroomStation::Shower)) | occupants) return false;
    return !(stations & static_cast<uint8_t>(BathroomStation::Shower));
}

} // namespace bathroom_api

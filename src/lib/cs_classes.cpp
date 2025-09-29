#include "cs_classes.h"

namespace bathroom_api {

// Bathroom constructor
Bathroom::Bathroom(uint8_t stations_, uint8_t occupants_) {
    stations = stations_;
    occupants = occupants_;
}

// Check if a station is free
inline bool Bathroom::isFree(BathroomStation station) const {
    return !(static_cast<uint8_t>(station) & stations);
}

// Take a station
void Bathroom::takeStation(BathroomStation station, Person user) {
    stations |= static_cast<uint8_t>(station);
    occupants |= static_cast<uint8_t>(user);
}

// Release a station
void Bathroom::releaseStation(BathroomStation station, Person user) {
    stations &= ~static_cast<uint8_t>(station);
    occupants &= ~static_cast<uint8_t>(user);
}

// Check availability of a station according to your rules
inline bool Bathroom::stationAvailable(BathroomStation station, Person user) const {
    constexpr uint8_t momDadMask = static_cast<uint8_t>(Person::Mom) | static_cast<uint8_t>(Person::Dad);
    if (!isFree(station)) return false;
    if (!stations) return true;
    // Mom + Dad logic
    if ((static_cast<uint8_t>(user) & momDadMask) && !(occupants & ~momDadMask)) return true;
    // Shower logic
    if ((static_cast<uint8_t>(station) & ~static_cast<uint8_t>(BathroomStation::Shower)) | occupants) return false;
    return !(stations & static_cast<uint8_t>(BathroomStation::Shower));
}

inline bool Bathroom::isUsingStation(Person user) const {
    return occupants & static_cast<uint8_t>(user);
}

} // namespace bathroom_api

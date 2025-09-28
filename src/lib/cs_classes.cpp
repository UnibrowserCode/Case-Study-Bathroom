#include "cs_classes.h"

namespace bathroom_api {

// Bathroom constructor
Bathroom::Bathroom(uint8_t stations_, uint8_t occupants_) {
    stations = stations_;
    occupants = occupants_;
}

// Check if a station is free
bool Bathroom::isFree(BathroomStation station) const {
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
bool Bathroom::stationAvailable(BathroomStation station, Person user) const {
    if (!isFree(station)) return false;
    if (stations == 0) return true;
    // Mom + Dad logic: only Mom and Dad in bathroom
    if ((occupants | static_cast<uint8_t>(user)) <= 3) return true;
    // Otherwise, normal shower rule
    return stations & static_cast<uint8_t>(BathroomStation::Shower) ? false : true;
}

} // namespace bathroom_api

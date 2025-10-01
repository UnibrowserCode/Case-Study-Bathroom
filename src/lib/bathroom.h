#pragma once
#include "globals.h"
#include "person.h"
#include <array>


namespace bathroomAPI {

enum class BathroomStation : uint8_t;
class Schedule;

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
    Bathroom(uint8_t stations_ = 0, uint8_t occupants_ = 0, std::array<personAPI::Person, 8> occupation_ = {});
    inline bool isFree(BathroomStation station) const;
    void takeStation(BathroomStation station, personAPI::PersonName user);
    void releaseStation(BathroomStation station, personAPI::PersonName user);
    inline bool stationAvailable(BathroomStation station, personAPI::PersonName user) const;
    inline bool isUsingStation(personAPI::PersonName user) const;
};

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

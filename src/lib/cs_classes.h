#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <iostream>

namespace bathroom_api {

// Templates (inline must stay in header)
template <typename T>
inline void print(T data) {
    std::cout << data << "\n";
}

template <typename T>
inline void debug(T data) {
    std::cout << "DEBUG: " << data << "\n";
}

// Forward declarations
enum class Person : uint8_t;
enum class BathroomStation : uint8_t;
class Schedule;

// Person enum
enum class Person : uint8_t {
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

// BathroomStation enum
enum class BathroomStation : uint8_t {
    None   = 0,
    Sink1  = 1 << 0,
    Sink2  = 1 << 1,
    Tub    = 1 << 2,
    Shower = 1 << 3
};

// Bathroom class declaration
class Bathroom {
public:  // <--- stations and occupants public
    uint8_t stations;
    uint8_t occupants;

public:
    Bathroom(uint8_t stations_ = 0, uint8_t occupants_ = 0);

    bool isFree(BathroomStation station) const;
    void takeStation(BathroomStation station, Person user);
    void releaseStation(BathroomStation station, Person user);
    bool stationAvailable(BathroomStation station, Person user) const;
};

// Schedule class
class Schedule {};

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

inline Person operator|(Person a, Person b) {
    return static_cast<Person>(
        static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

inline Person operator&(Person a, Person b) {
    return static_cast<Person>(
        static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}

inline Person& operator|=(Person& a, Person b) {
    return a = a | b;
}

} // namespace bathroom_api

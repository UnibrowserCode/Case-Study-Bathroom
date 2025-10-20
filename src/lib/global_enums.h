#pragma once
#include "common_includes.h"
namespace bathroomAPI {
enum class BathroomStation : uint8_t {
    None   = 0,
    Sink1  = 1 << 0,
    Sink2  = 1 << 1,
    Tub    = 1 << 2,
    Shower = 1 << 3,
    Meeting = 1 << 4
};
constexpr inline uint8_t operator|=(uint8_t &lhs, BathroomStation rhs) noexcept {
    return lhs = static_cast<uint8_t>(rhs) | lhs;
}
constexpr inline uint8_t operator&=(uint8_t &lhs, BathroomStation rhs) noexcept {
    return lhs = static_cast<uint8_t>(rhs) & lhs;
}
constexpr inline uint8_t operator^=(uint8_t &lhs, BathroomStation rhs) noexcept {
    return lhs = static_cast<uint8_t>(rhs) ^ lhs;
}
constexpr inline BathroomStation operator|(BathroomStation lhs, BathroomStation rhs) noexcept {
    return static_cast<BathroomStation>(
        static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs)
    );
}
constexpr inline BathroomStation operator&(BathroomStation lhs, BathroomStation rhs) noexcept {
    return static_cast<BathroomStation>(
        static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs)
    );
}
constexpr inline BathroomStation operator^(BathroomStation lhs, BathroomStation rhs) noexcept {
    return static_cast<BathroomStation>(
        static_cast<uint8_t>(lhs) ^ static_cast<uint8_t>(rhs)
    );
}
constexpr inline BathroomStation operator~(BathroomStation value) noexcept {
    return static_cast<BathroomStation>(
        ~static_cast<uint8_t>(value)
    );
}
constexpr inline BathroomStation& operator|=(BathroomStation& lhs, BathroomStation rhs) noexcept {
    return lhs = lhs | rhs;
}
constexpr inline BathroomStation& operator&=(BathroomStation& lhs, BathroomStation rhs) noexcept {
    return lhs = lhs & rhs;
}
constexpr inline BathroomStation& operator^=(BathroomStation& lhs, BathroomStation rhs) noexcept {
    return lhs = lhs ^ rhs;
}
constexpr inline uint8_t operator&(uint8_t lhs, BathroomStation rhs) noexcept {
    return lhs & static_cast<uint8_t>(rhs);
}
constexpr inline uint8_t operator|(uint8_t lhs, BathroomStation rhs) noexcept {
    return lhs | static_cast<uint8_t>(rhs);
}
constexpr inline uint8_t operator^(uint8_t lhs, BathroomStation rhs) noexcept {
    return lhs ^ static_cast<uint8_t>(rhs);
}
constexpr inline uint8_t operator&(BathroomStation lhs, uint8_t rhs) noexcept {
    return static_cast<uint8_t>(lhs) & rhs;
}
constexpr inline uint8_t operator|(BathroomStation lhs, uint8_t rhs) noexcept {
    return static_cast<uint8_t>(lhs) | rhs;
}
constexpr inline uint8_t operator^(BathroomStation lhs, uint8_t rhs) noexcept {
    return static_cast<uint8_t>(lhs) ^ rhs;
}
} // namespace bathroomAPI

namespace personAPI {
// Person enum
enum class PersonName : uint8_t {
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
constexpr inline uint8_t operator|=(uint8_t &lhs, PersonName rhs) noexcept {
    return lhs = static_cast<uint8_t>(rhs) | lhs;
}
constexpr inline uint8_t operator&=(uint8_t &lhs, PersonName rhs) noexcept {
    return lhs = static_cast<uint8_t>(rhs) & lhs;
}
constexpr inline PersonName operator|(PersonName lhs, PersonName rhs) noexcept {
    return static_cast<PersonName>(
        static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs)
    );
}
constexpr inline PersonName operator&(PersonName lhs, PersonName rhs) noexcept {
    return static_cast<PersonName>(
        static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs)
    );
}
constexpr inline PersonName operator^(PersonName lhs, PersonName rhs) noexcept {
    return static_cast<PersonName>(
        static_cast<uint8_t>(lhs) ^ static_cast<uint8_t>(rhs)
    );
}
constexpr inline PersonName operator~(PersonName value) noexcept {
    return static_cast<PersonName>(
        ~static_cast<uint8_t>(value)
    );
}
constexpr inline PersonName& operator|=(PersonName& lhs, PersonName rhs) noexcept {
    return lhs = lhs | rhs;
}
constexpr inline PersonName& operator&=(PersonName& lhs, PersonName rhs) noexcept {
    return lhs = lhs & rhs;
}
constexpr inline PersonName& operator^=(PersonName& lhs, PersonName rhs) noexcept {
    return lhs = lhs ^ rhs;
}
constexpr inline uint8_t operator&(uint8_t lhs, PersonName rhs) noexcept {
    return lhs & static_cast<uint8_t>(rhs);
}
constexpr inline uint8_t operator|(uint8_t lhs, PersonName rhs) noexcept {
    return lhs | static_cast<uint8_t>(rhs);
}
constexpr inline uint8_t operator^(uint8_t lhs, PersonName rhs) noexcept {
    return lhs ^ static_cast<uint8_t>(rhs);
}
constexpr inline uint8_t operator&(PersonName lhs, uint8_t rhs) noexcept {
    return static_cast<uint8_t>(lhs) & rhs;
}
constexpr inline uint8_t operator|(PersonName lhs, uint8_t rhs) noexcept {
    return static_cast<uint8_t>(lhs) | rhs;
}
constexpr inline uint8_t operator^(PersonName lhs, uint8_t rhs) noexcept {
    return static_cast<uint8_t>(lhs) ^ rhs;
}
} // namespace personAPI
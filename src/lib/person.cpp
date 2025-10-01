#include "globals.h"
#include "bathroom.h"
#include "person.h"
#include <array>


namespace personAPI {

inline personAPI::PersonName operator|(personAPI::PersonName a, personAPI::PersonName b) {
    return static_cast<personAPI::PersonName>(
        static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

inline personAPI::PersonName operator&(personAPI::PersonName a, personAPI::PersonName b) {
    return static_cast<personAPI::PersonName>(
        static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}

inline personAPI::PersonName& operator|=(personAPI::PersonName& a, personAPI::PersonName b) {
    return a = a | b;
}

} // namespace person_api
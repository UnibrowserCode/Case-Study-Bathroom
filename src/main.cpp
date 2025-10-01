#include "lib/bathroom.h"
#include "lib/schedule.h"
#include "lib/person.h"
#include <algorithm>
#include <string>
#include <array>
#include <iostream>

using namespace std;
using namespace bathroomAPI;
using namespace personAPI;


//| vvv Utilities vvv |//
template <typename T>
void print(T data) {
    std::cout << data << "\n";
}

template <typename T>
void debug(T data) {
    std::cout << "DEBUG: " << data << "\n";
}


//| vvv Generation vvv |//
constexpr array<Person, 8> makeFamily() {
    return {{
        {personAPI::PersonName::Dad,    BathroomStation::None, 0,
            std::array<Task, 2>{{
                {BathroomStation::Shower, 10, false},
                {BathroomStation::Sink1,  18, false}
            }}
        },
        {personAPI::PersonName::Mom,    BathroomStation::None, 0,
            std::array<Task, 2>{{
                {BathroomStation::Shower, 17, false},
                {BathroomStation::Sink1,  30, false}
            }}
        },
        { personAPI::PersonName::Heather,BathroomStation::None, 0,
            std::array<Task, 2>{{
                {BathroomStation::Shower, 13, false},
                {BathroomStation::Sink1,  44, false}
            }}
        },
        {personAPI::PersonName::Nick,   BathroomStation::None, 0,
            std::array<Task, 2>{{
                {BathroomStation::Shower, 3, false},
                {BathroomStation::Sink1, 32, false}
            }}
        },
        {personAPI::PersonName::Rulon,  BathroomStation::None, 0,
            std::array<Task, 2>{{
                {BathroomStation::Shower, 7, false},
                {BathroomStation::Sink1, 18, false}
            }}
        },
        {personAPI::PersonName::George, BathroomStation::None, 0,
            std::array<Task, 2>{{
                {BathroomStation::Shower, 3, false},
                {BathroomStation::Sink1,  5, false}
            }}
        },
        {personAPI::PersonName::Olivia, BathroomStation::None, 0,
            std::array<Task, 2>{{
                {BathroomStation::Shower, 15, false},
                {BathroomStation::Sink1,   9, false}
            }}
        },
        {personAPI::PersonName::Thomas, BathroomStation::None, 0,
            std::array<Task, 2>{{
                {BathroomStation::Tub,   25, false},
                {BathroomStation::Sink1,  3, false}
            }}
        }
    }};
}

constexpr array<BathroomStation, 4> makeStations() {
    return {
        BathroomStation::Shower,
        BathroomStation::Sink1,
        BathroomStation::Sink2,
        BathroomStation::Tub,
    };
}

inline constexpr auto cmpByName = [](const Person& a, const Person& b) {
    return static_cast<int>(a.name) < static_cast<int>(b.name);
};

inline constexpr auto cmpByStation = [](const BathroomStation& a, const BathroomStation& b) {
    return static_cast<int>(a) < static_cast<int>(b);
};

scheduleAPI::Day planDay(array<BathroomStation, 4> _stations, array<Person, 8> _people) {
    scheduleAPI::Day day = {};
    auto family = makeFamily();
    auto stations = makeStations();
    // Sort once so next_permutation covers all possible permutations
    sort(family.begin(), family.end(), cmpByName);
    int count = 0;
    do {
        for (auto& person : family) {
            sort(stations.begin(), stations.end(), cmpByStation);
            do {
                for (auto& station : stations) {
                    
                    ++count;
                }
            } while (next_permutation(stations.begin(), stations.end(), cmpByStation));
        }
    } while (next_permutation(family.begin(), family.end(), cmpByName));

    cout << "Total permutations: " << count << "\n";
    return day;
}


int main() {
    Bathroom bathroom;
    Person mom     = {personAPI::PersonName::Mom,     BathroomStation::None, 0, {}};
    Person dad     = {personAPI::PersonName::Dad,     BathroomStation::None, 0, {}};
    Person heather = {personAPI::PersonName::Heather, BathroomStation::None, 0, {}};
    Person nick    = {personAPI::PersonName::Nick,    BathroomStation::None, 0, {}};
    Person rulon   = {personAPI::PersonName::Rulon,   BathroomStation::None, 0, {}};
    Person george  = {personAPI::PersonName::George,  BathroomStation::None, 0, {}};
    Person olivia  = {personAPI::PersonName::Olivia,  BathroomStation::None, 0, {}};
    Person thomas  = {personAPI::PersonName::Thomas,  BathroomStation::None, 0, {}};
    array<Person, 8> people = {mom, dad, heather, nick, rulon, george, olivia, thomas};
    array<BathroomStation, 4> stations = {
        BathroomStation::Sink1,
        BathroomStation::Sink2,
        BathroomStation::Tub,
        BathroomStation::Shower
    };

    scheduleAPI::Week schedule = {};
    planDay(stations, people);
    // for (int i = 0; i < sizeof())
    return 0;
}

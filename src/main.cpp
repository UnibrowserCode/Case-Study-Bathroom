#include "lib/bathroom.h"
#include "lib/schedule.h"
#include "lib/person.h"
#include <algorithm>
#include <string>
#include <array>
#include <iostream>
#include <chrono>

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

inline string formatWithCommas(long long value) {
    string num = to_string(value);
    int insertPosition = num.length() - 3;
    while (insertPosition > 0) {
        num.insert(insertPosition, ",");
        insertPosition -= 3;
    }
    return num;
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
    sort(stations.begin(), stations.end(), cmpByStation);
    Bathroom bathroom = Bathroom();
    for (int i = 0; i < 120; i++) {
        do {
            for (auto& person : family) {
                person.timeLeftUsing -= 1 * (person.timeLeftUsing > 0);
                Task personTask1 = person.tasks[0];
                Task personTask2 = person.tasks[1];
                do {
                    for (auto& station : stations) {
                        uint8_t matchingStation = (static_cast<uint8_t>(personTask1.station) | static_cast<uint8_t>(personTask2.station)) & static_cast<uint8_t>(station);
                        if (!person.timeLeftUsing && !(personTask1.completed && personTask2.completed) && (matchingStation)) {
                            if (matchingStation & static_cast<uint8_t>(personTask1.station) && !personTask1.completed && bathroom.stationAvailable(personTask1.station, person.name)) {
                                bathroom.takeStation(personTask1.station, person.name);
                                person.timeLeftUsing = personTask1.timeRequired;
                                person.curStation = personTask1.station;
                            } else if (!personTask2.completed && bathroom.stationAvailable(personTask2.station, person.name)) {
                                bathroom.takeStation(personTask2.station, person.name);
                                person.timeLeftUsing = personTask2.timeRequired;
                                person.curStation = personTask2.station;
                            }
                        } else {
                            if (!static_cast<int>(person.curStation) && !person.timeLeftUsing) {
                                bathroom.releaseStation(person.curStation, person.name);
                                person.tasks[personTask1.station == person.curStation].completed = true;
                                person.curStation = BathroomStation::None;
                            }
                        }
                    }
                } while (next_permutation(stations.begin(), stations.end(), cmpByStation));
            }
        } while (next_permutation(family.begin(), family.end(), cmpByName));
    }
    cout << "Total iterations: 3,715,891,200\n";
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
    auto start = chrono::high_resolution_clock::now();
    planDay(stations, people);
    auto end = chrono::high_resolution_clock::now();
    auto duration_second = chrono::duration_cast<chrono::seconds>(end - start);
    auto duration_ms = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Took " << duration_second.count() << " seconds\n";
    cout << "Took " << formatWithCommas(duration_ms.count()) << " milliseconds\n";
    cout << "(" << formatWithCommas(3715891200 / duration_ms.count()) << " calculations/ms)\n";
    // for (int i = 0; i < sizeof())
    return 0;
}

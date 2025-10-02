#include "lib/bathroom.h"
#include "lib/schedule.h"
#include "lib/person.h"
#include <algorithm>
#include <string>
#include <array>
#include <iostream>
#include <chrono>
#include <fstream>

using namespace std;
using namespace bathroomAPI;
using namespace personAPI;


//| vvv Classes & Structs vvv |//



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
inline constexpr std::array<Person, 8> makeFamily() {
    return {
        Person{PersonName::Dad, BathroomStation::None, 0,
               std::array<Task, 2>{{
                   {BathroomStation::Shower, 10, false},
                   {BathroomStation::Sink1, 18, false}
               }}},
        Person{PersonName::Mom, BathroomStation::None, 0,
               std::array<Task, 2>{{
                   {BathroomStation::Shower, 17, false},
                   {BathroomStation::Sink1, 30, false}
               }}},
        Person{PersonName::Heather, BathroomStation::None, 0,
               std::array<Task, 2>{{
                   {BathroomStation::Shower, 13, false},
                   {BathroomStation::Sink1, 44, false}
               }}},
        Person{PersonName::Nick, BathroomStation::None, 0,
               std::array<Task, 2>{{
                   {BathroomStation::Shower, 3, false},
                   {BathroomStation::Sink1, 32, false}
               }}},
        Person{PersonName::Rulon, BathroomStation::None, 0,
               std::array<Task, 2>{{
                   {BathroomStation::Shower, 7, false},
                   {BathroomStation::Sink1, 18, false}
               }}},
        Person{PersonName::George, BathroomStation::None, 0,
               std::array<Task, 2>{{
                   {BathroomStation::Shower, 3, false},
                   {BathroomStation::Sink1, 5, false}
               }}},
        Person{PersonName::Olivia, BathroomStation::None, 0,
               std::array<Task, 2>{{
                   {BathroomStation::Shower, 15, false},
                   {BathroomStation::Sink1, 9, false}
               }}},
        Person{PersonName::Thomas, BathroomStation::None, 0,
               std::array<Task, 2>{{
                   {BathroomStation::Tub, 25, false},
                   {BathroomStation::Sink1, 3, false}
               }}}
    };
}

array<Person, 8> family = makeFamily();

array<BathroomStation, 4> stations = {
    BathroomStation::Shower,
    BathroomStation::Sink1,
    BathroomStation::Sink2,
    BathroomStation::Tub,
};

inline auto cmpByNameIdx = [](int a, int b) {
    return static_cast<int>(family[a].name) < static_cast<int>(family[b].name);
};

inline auto cmpByStationIdx = [](int a, int b) {
    return static_cast<int>(stations[a]) < static_cast<int>(stations[b]);
};

inline scheduleAPI::Day simulateDay(array<int, 8> &familyIdxs, array<int, 4> &stationIdxs) {
    scheduleAPI::Day day = {};
    Bathroom bathroom;
    for (int minute = 0; minute < 120; minute++) {
        for (int &personIdx : familyIdxs) {
            Person &person = family[personIdx];
            person.timeLeftUsing -= person.timeLeftUsing > 0;
            Task personTask1 = person.tasks[0];
            Task personTask2 = person.tasks[1];
            if (!person.timeLeftUsing && !person.allTasksCompleted()) {
                for (int &stationIdx : stationIdxs) {
                    BathroomStation &station = stations[stationIdx];
                    bool matchingStation = (static_cast<uint8_t>(personTask1.station) | static_cast<uint8_t>(personTask2.station)) & static_cast<uint8_t>(station);
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
            }
        }
        day.minutes[minute] = bathroom;
    }
    return day;
}

inline float scoreDay(scheduleAPI::Day day) {
    Bathroom result = day.minutes[120];
    for (auto &person : result.occupation) {
        if (!person.allTasksCompleted()) {
            return 0.0f;
        }
    }
    cout << "Found day with correct schedule!\n";
    return 10.0f;
}


scheduleAPI::Day planDay() {
    scheduleAPI::Day bestDay = {};
    float bestDayScore = -1;
    array<int, 8> familyIdxs = {0, 1, 2, 3, 4, 5, 6, 7};
    array<int, 4> stationIdxs = {0, 1, 2, 3};
    // Sort once so next_permutation covers all possible permutations
    Bathroom bathroom = Bathroom();
    unsigned long int count = 0;
    do {
        do {
            auto day = simulateDay(familyIdxs, stationIdxs);
            auto curScore = scoreDay(day);
            if (curScore > bestDayScore) {
                bestDayScore = curScore;
                bestDay.minutes = day.minutes;
            }
        } while (next_permutation(stationIdxs.begin(), stationIdxs.end(), cmpByStationIdx));
    } while (next_permutation(familyIdxs.begin(), familyIdxs.end(), cmpByNameIdx));
    cout << "Total iterations: " << formatWithCommas(count) << "\n";
    return bestDay;
}

inline string formatBathroom(Bathroom bathroom) {
    string output = "";
    for (Person occupant : bathroom.occupation) {
        output += to_string(occupant.name) + ": " + to_string(occupant.curStation);
    }
    return output;
}

inline string formatDay(scheduleAPI::Day day) {
    string output = "";
    for (Bathroom bathroom : day.minutes) {
        output += formatBathroom(bathroom) + "\n";
    }
    return output;
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

    scheduleAPI::Week schedule = {};
    auto start = chrono::high_resolution_clock::now();
    scheduleAPI::Day day = planDay();
    auto end = chrono::high_resolution_clock::now();
    auto duration_second = chrono::duration_cast<chrono::seconds>(end - start);
    auto duration_ms = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Took " << duration_second.count() << " seconds\n";
    cout << "Took " << formatWithCommas(duration_ms.count()) << " milliseconds\n";
    cout << "(" << formatWithCommas(3715891200 / duration_ms.count()) << " calculations/ms)\n";
    ofstream outputFile("save.txt");
    outputFile.open("save.txt");
    if (outputFile.is_open()) {
        outputFile << formatDay(day);
        cout << "Saved file!";
    }
    // for (int i = 0; i < sizeof())
    return 0;
}

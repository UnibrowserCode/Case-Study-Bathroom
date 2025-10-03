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
inline std::array<Person, 8> makeFamily() {
    return {
        Person{PersonName::Dad, BathroomStation::None, 0,
               {BathroomStation::Shower, 10, false},
               {BathroomStation::Sink1, 18, false}},
        Person{PersonName::Mom, BathroomStation::None, 0,
               {BathroomStation::Shower, 17, false},
               {BathroomStation::Sink1, 30, false}},
        Person{PersonName::Heather, BathroomStation::None, 0,
               {BathroomStation::Shower, 13, false},
               {BathroomStation::Sink1, 44, false}},
        Person{PersonName::Nick, BathroomStation::None, 0,
               {BathroomStation::Shower, 3, false},
               {BathroomStation::Sink1, 32, false}},
        Person{PersonName::Rulon, BathroomStation::None, 0,
               {BathroomStation::Shower, 7, false},
               {BathroomStation::Sink1, 18, false}},
        Person{PersonName::George, BathroomStation::None, 0,
               {BathroomStation::Shower, 3, false},
               {BathroomStation::Sink1, 5, false}},
        Person{PersonName::Olivia, BathroomStation::None, 0,
               {BathroomStation::Shower, 15, false},
               {BathroomStation::Sink1, 9, false}},
        Person{PersonName::Thomas, BathroomStation::None, 0,
               {BathroomStation::Tub, 25, false},
               {BathroomStation::Sink1, 3, false}}
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

inline scheduleAPI::Day simulateDay(array<int, 8> &familyIdxs, array<int, 4> &stationIdxs, array<Person, 8> &family) {
    scheduleAPI::Day day = {};
    Bathroom bathroom;
    array<Person, 8> tempFamily = family; // work on this instead of family

    for (int minute = 0; minute < 120; ++minute) {
        for (int personIdx = 0; personIdx < 8; personIdx++) {
            Person &person = tempFamily[personIdx]; // changed to tempFamily
            Task &task1 = person.task1;
            Task &task2 = person.task2;

            // decrement time if using a station
            if (person.curStation != BathroomStation::None) {
                    person.timeLeftUsing -= 1;
                if (person.timeLeftUsing <= 0) {
                    // cout << "RELEASING " << to_string(person.curStation) << "\n";
                    bathroom.releaseStation(person.curStation, person.name);
                    if (task1.station == person.curStation)
                        task1.completed = true;
                    if (task2.station == person.curStation)
                        task2.completed = true;
                    person.curStation = BathroomStation::None;
                }
                bathroom.occupation[personIdx] = person;
            }
            if (!person.timeLeftUsing && !person.allTasksCompleted()) {
                for (int &stationIdx : stationIdxs) {
                    BathroomStation station = stations[stationIdx];
                    // check task1 first if its station is available
                    if (!task1.completed && task1.station == station && bathroom.stationAvailable(task1.station, person.name)) {
                        bathroom.takeStation(task1.station, person.name);
                        person.timeLeftUsing = task1.timeRequired;
                        person.curStation = task1.station;
                        // cout << to_string(person.name) << " TOOK: " << to_string(task1.station) << " FOR " << person.timeLeftUsing << "\n";
                        bathroom.occupation[personIdx] = person;
                        break;
                    } else if (!task2.completed && task2.station == station && bathroom.stationAvailable(task2.station, person.name)) {
                        bathroom.takeStation(task2.station, person.name);
                        person.timeLeftUsing = task2.timeRequired;
                        person.curStation = task2.station;
                        // cout << to_string(person.name) << " TOOK: " << to_string(task2.station) << " FOR " << person.timeLeftUsing << "\n";
                        bathroom.occupation[personIdx] = person;
                        break;
                    }                    
                }
            }
        }
        // store bathroom state for this minute
        day.minutes[minute] = bathroom;
    }
    return day;
}

inline float scoreDay(const scheduleAPI::Day &day) {
    // Check the last minute for completion
    const Bathroom &finalState = day.minutes.back(); // last minute
    for (const auto& person : finalState.occupation) {
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
    unsigned long long count = 0;
    array<Person, 8> family = makeFamily();

    sort(familyIdxs.begin(), familyIdxs.end(), cmpByNameIdx);
    sort(stationIdxs.begin(), stationIdxs.end(), cmpByStationIdx);
    do {
        do {
            auto day = simulateDay(familyIdxs, stationIdxs, family);
            auto curScore = scoreDay(day);

            if (curScore > bestDayScore) {
                bestDayScore = curScore;
                bestDay.minutes = day.minutes;
            }
            ++count;
        } while (next_permutation(stationIdxs.begin(), stationIdxs.end(), cmpByStationIdx));
    } while (next_permutation(familyIdxs.begin(), familyIdxs.end(), cmpByNameIdx));

    cout << "Total iterations: " << formatWithCommas(count) << "\n";
    return bestDay;
}

inline string formatBathroom(const Bathroom &bathroom) {
    string output;
    for (const Person& occupant : bathroom.occupation) {
        output += to_string(occupant.name) + ": " + to_string(occupant.curStation) + ",  ";
    }
    return output;
}

inline string formatDay(scheduleAPI::Day &day) {
    string output = "";
    for (const Bathroom &bathroom : day.minutes) {
        output += formatBathroom(bathroom) + "\n";
    }
    return output;
}


int main() {
    Bathroom bathroom;

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
    if (outputFile.is_open()) {
        outputFile << formatDay(day);
        cout << "Saved file!";
    }
    // for (int i = 0; i < sizeof())
    return 0;
}

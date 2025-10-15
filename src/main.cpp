#include "lib/bathroom.h"
#include "lib/schedule.h"
#include "lib/person.h"
#include <algorithm>
#include <string>
#include <array>
#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

using namespace std;
using namespace bathroomAPI;
using namespace personAPI;


//| vvv Classes & Structs vvv |//


// MARK: Utilites

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


// MARK: Person Gen
//| vvv Generation vvv |//
inline array<Person, 8> baseFamily() {
    return {
        Person{PersonName::Dad, BathroomStation::None, 0,
               {BathroomStation::Shower, 0, false},  // 10
               {BathroomStation::Sink1, 18, false}},
        Person{PersonName::Mom, BathroomStation::None, 0,
               {BathroomStation::Shower, 0, false},  // 17
               {BathroomStation::Sink2, 30, false}},
        Person{PersonName::Heather, BathroomStation::None, 0,
               {BathroomStation::Shower, 0, false},  // 13
               {BathroomStation::Sink1, 44, false}},
        Person{PersonName::Nick, BathroomStation::None, 0,
               {BathroomStation::Shower, 0, false},  // 3
               {BathroomStation::Sink2, 32, false}},
        Person{PersonName::Rulon, BathroomStation::None, 0,
               {BathroomStation::Shower, 0, false},  // 7
               {BathroomStation::Sink1, 18, false}},
        Person{PersonName::George, BathroomStation::None, 0,
               {BathroomStation::Shower, 0, false},  // 3
               {BathroomStation::Sink2, 5, false}},
        Person{PersonName::Olivia, BathroomStation::None, 0,
               {BathroomStation::Shower, 0, false},  // 15
               {BathroomStation::Sink1, 9, false}},
        Person{PersonName::Thomas, BathroomStation::None, 0,
               {BathroomStation::Tub, 20, false},
               {BathroomStation::Sink2, 0, true}}
    };
}


// MARK: GLobals
array<Person, 8> family;

array<BathroomStation, 4> stations = {
    BathroomStation::Shower,
    BathroomStation::Sink1,
    BathroomStation::Sink2,
    BathroomStation::Tub,
};


// MARK: Sim Day
inline scheduleAPI::Day simulateDay(array<int, 8> &familyIdxs, array<int, 4> &stationIdxs, const array<Person, 8> &family, const short &showerCutoff) {
    scheduleAPI::Day day = {};
    Bathroom bathroom;
    array<Person, 8> tempFamily = family; // work on this instead of family
    constexpr uint8_t showerTubMask = static_cast<uint8_t>(BathroomStation::Shower) | static_cast<uint8_t>(BathroomStation::Tub);
    constexpr uint8_t momDadMask = static_cast<uint8_t>(PersonName::Mom) | static_cast<uint8_t>(PersonName::Dad);
    for (int personIdx = 0; personIdx < 8; personIdx++) {
        Person &person = tempFamily[personIdx];
        if (person.task1.station == BathroomStation::Shower) {
            person.task1.completed = true;
        }
        else if (person.task2.station == BathroomStation::Shower) {
            person.task2.completed = true;
        }
    }

    for (int minute = 0; minute < day.minutesInDay; ++minute) {
        for (int personIdx = 0; personIdx < 8; personIdx++) {
            Person &person = tempFamily[personIdx]; // changed to tempFamily
            uint8_t personUint = static_cast<uint8_t>(person.name);
            Task &task1 = person.task1;
            Task &task2 = person.task2;

            // decrement time if using a station
            if (person.curStation != BathroomStation::None) {
                    person.timeLeftUsing -= 1;
                if (person.timeLeftUsing <= 0) {
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
                    if (station == BathroomStation::Shower) break;
                    if (minute < showerCutoff) {
                        // if (person.name == PersonName::Dad) break;
                        if (static_cast<uint8_t>(station) & showerTubMask) break;
                        if (personUint & momDadMask) break;
                    }
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

// MARK: Score Day
inline float scoreDay(const scheduleAPI::Day &day, const short &showerCutoff) {
    // Check the last minute for completion
    float score = 0;
    const Bathroom &finalState = day.minutes.back(); // last minute
    for (const auto &person : finalState.occupation) {
        // FIXME: Make sure task1 and task2 aren't shower
        score += person.task1.completed + person.task2.completed;
    }
    uint8_t showerUint = static_cast<uint8_t>(BathroomStation::Shower);
    short minuteIdx = 0;
    for (const auto &minute : day.minutes) {
        score -= (minute.stations & showerUint != 0 && minuteIdx < showerCutoff) / 100;
        score += (minute.stations != 0) / 100;
        minuteIdx++;
    }
    return score;
}


// MARK: Gen Sink
vector<array<Person, 8>> generateSinkCombinations() {
    vector<array<Person, 8>> combinations;
    const int numPeople = 7; // excluding Thomas
    int maxChoice = 1;       // 0 = Sink1, 1 = Sink2

    array<int, numPeople> choices = {}; // all start at Sink1

    while (true) {
        int sink1Count = 0, sink2Count = 0;
        for (int c : choices) {
            if (c == 0) sink1Count++;
            else sink2Count++;
        }
        array<Person, 8> fam = baseFamily();
        // Assign sink choice to each person (0..6)
        for (int i = 0; i < numPeople; ++i) {
            fam[i].task2.station = choices[i] == 0 ? BathroomStation::Sink1
                                                   : BathroomStation::Sink2;
        }
        // Thomas always has Sink2 (as in your original baseFamily)
        fam[7].task2.station = BathroomStation::Sink2;

        combinations.push_back(fam);
        // Increment binary counter
        int idx = 0;
        while (idx < numPeople) {
            if (choices[idx] < maxChoice) {
                choices[idx]++;
                break;
            } else {
                choices[idx] = 0;
                idx++;
            }
        }
        if (idx == numPeople) break; // finished all combinations
    }
    return combinations;
}

// MARK: Test Days
scheduleAPI::Day testDays(const array<Person, 8> &family, const short &timeTillShower) {
    scheduleAPI::Day bestDay = {};
    float bestDayScore = -1;
    array<int, 8> familyIdxs = {0, 1, 2, 3, 4, 5, 6, 7};
    array<int, 4> stationIdxs = {0, 1, 2, 3};
    unsigned long count = 0;

    sort(familyIdxs.begin(), familyIdxs.end());
    sort(stationIdxs.begin(), stationIdxs.end());
    do {
        do {
            auto day = simulateDay(familyIdxs, stationIdxs, family, timeTillShower);
            auto curScore = scoreDay(day, timeTillShower);

            if (curScore > bestDayScore) {
                bestDayScore = curScore;
                bestDay.minutes = day.minutes;
            }
            count++;
        } while (next_permutation(stationIdxs.begin(), stationIdxs.end()));
    } while (next_permutation(familyIdxs.begin(), familyIdxs.end()));

    cout << "\nTotal iterations: " << formatWithCommas(count) << "\n";
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

mutex mtx;
scheduleAPI::Day correctDay;
int correctDayScore;
atomic<bool> found = false;

// MARK: Worker
void worker(const array<Person, 8> &family, const short &timeTillShower) {
    array<Person, 8> familyCopy = family;  // local copy for thread safety
    scheduleAPI::Day day = testDays(familyCopy, timeTillShower);
    int score = scoreDay(day, timeTillShower);
    lock_guard<mutex> guard(mtx);
    if (!found.load()) { 
        if (score > correctDayScore) {
            correctDay.minutes = day.minutes;  // save the solution
            found = true;      // signal other threads
        }
    }
}

// MARK: Main
int main() {
    Bathroom bathroom;
    scheduleAPI::Day exampleDay;
    constexpr short midnightTillEnd = 480;     // total minutes from midnight to end of day (8 hours)
    constexpr short midnightTillShower = 405;  // minutes from midnight until 6:45 AM
    const short timeTillShower = midnightTillShower - (midnightTillEnd - exampleDay.minutesInDay);

    scheduleAPI::Week schedule = {};
    auto combos = generateSinkCombinations();
    cout << "Generated " << combos.size() << " family combinations.\n";
    vector<thread> threads;
    for (int i = 0; i < 128; i++) {
        threads.push_back(thread(worker, combos[i], timeTillShower));
        if (found) break;
    }
    for (auto &t : threads) {
        t.join();
    }
    if (found) {
        ofstream outputFile("save.txt");
        if (outputFile.is_open()) {
            outputFile << scoreDay(correctDay, timeTillShower) << "\n";
            outputFile << formatDay(correctDay);
            cout << "Score: " << scoreDay(correctDay, timeTillShower) << "16\n";
            cout << "Saved file!\n";
        }
        cout << correctDayScore << "\n";
        const Bathroom &finalState = correctDay.minutes.back(); // last minute
        for (const auto& person : finalState.occupation) {
            if (!person.task1.completed) {
                cout << to_string(person.name) << " did not complete " << to_string(person.task1.station) << "\n";
            }
            if (!person.task2.completed) {
                cout << to_string(person.name) << " did not complete " << to_string(person.task1.station) << "\n";
            }
        }
    } else {
        cout << "DID NOT FIND CORRECT DAY\n";
    }

    // for (int i = 0; i < sizeof())
    return 0;
}

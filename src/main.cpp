#include "lib/bathroom.h"
#include "lib/schedule.h"
#include "lib/person.h"
#include "lib/threadpool.h"
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

scheduleAPI::Day exampleDayGLOBAL;
struct TimeDataHolder {
    unsigned short timeTillEnd = 480;
    unsigned short timeDadHeatherNickLeave = 460;
    unsigned short timeTillRulonGeorgeLeave = 470;
    unsigned short startForAllShowers = 307;  // minutes from midnight until 5:12 AM (minimum required to fit all showers)
    unsigned short startOfDay = 0; // 350
    unsigned short timeTillShower = 405;  // minutes from midnight until 6:45 AM
    unsigned short meetingStart = 405;  // minutes from midnight until 6:45 AM
    unsigned short meetingEnd = 410;  // minutes from midnight until 6:45 AM
};

struct SimResult {
    scheduleAPI::Day day;
    std::array<Person, 8> finalFamily;
};

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
        Person{
            PersonName::Dad, 
            BathroomStation::None, 0,
            {BathroomStation::Shower, 10, false},  // 10
            {BathroomStation::Sink1, 18, false},
            {BathroomStation::None, 5, false},
            BathroomStation::None
        },
        Person{
            PersonName::Mom,
            BathroomStation::None, 0,
            {BathroomStation::Shower, 17, false}, // 17
            {BathroomStation::Sink2, 30, false},
            {BathroomStation::None, 5, false},
            BathroomStation::None
        },
        Person{
            PersonName::Heather,
            BathroomStation::None, 0,
            {BathroomStation::Shower, 13, false},  // 13
            {BathroomStation::Sink1, 44, false},
            {BathroomStation::None, 5, false},
            BathroomStation::None
            },
        Person{
            PersonName::Nick, 
            BathroomStation::None, 0,
            {BathroomStation::Shower, 3, false},  // 3
            {BathroomStation::Sink2, 32, false},
            {BathroomStation::None, 5, false},
            BathroomStation::None
        },
        Person{
            PersonName::Rulon,
            BathroomStation::None, 0,
            {BathroomStation::Shower, 0, true},  // 7
            {BathroomStation::Sink1, 18, false},
            {BathroomStation::None, 5, false},
            BathroomStation::None
        },
        Person{
            PersonName::George, 
            BathroomStation::None, 0,
            {BathroomStation::Shower, 3, false},  // 3
            {BathroomStation::Sink2, 5, false},
            {BathroomStation::None, 5, false},
            BathroomStation::None
        },
        Person{
            PersonName::Olivia, 
            BathroomStation::None, 0,
            {BathroomStation::Shower, 0, true},  // 15
            {BathroomStation::Sink1, 9, false},
            {BathroomStation::None, 5, false},
            BathroomStation::None
        },
        Person{
            PersonName::Thomas, 
            BathroomStation::None, 0,
            {BathroomStation::Tub, 20, false},
            {BathroomStation::Sink2, 0, true},
            {BathroomStation::None, 5, false},
            BathroomStation::None
            }
    };
}


// MARK: GLobals
//| GLOBALS
array<Person, 8> family;

array<BathroomStation, 4> stations = {
    BathroomStation::Shower,
    BathroomStation::Sink1,
    BathroomStation::Sink2,
    BathroomStation::Tub,
};


TimeDataHolder timeData;


//| SIM DAY
// MARK: Sim Day
inline scheduleAPI::Day simulateDay(array<int, 8> &familyIdxs, array<int, 4> &stationIdxs, const array<Person, 8> &family) {
    scheduleAPI::Day day = {};
    Bathroom bathroom;
    array<Person, 8> tempFamily = family; // work on this instead of family
    constexpr uint8_t showerTubMask = static_cast<uint8_t>(BathroomStation::Shower) | static_cast<uint8_t>(BathroomStation::Tub);
    constexpr uint8_t momDadMask = static_cast<uint8_t>(PersonName::Mom) | static_cast<uint8_t>(PersonName::Dad);
    constexpr uint8_t dadHeatherNickMask = static_cast<uint8_t>(PersonName::Dad) | static_cast<uint8_t>(PersonName::Heather) | static_cast<uint8_t>(PersonName::Nick);
    constexpr uint8_t rulonGeorgeMask = static_cast<uint8_t>(PersonName::Rulon) | static_cast<uint8_t>(PersonName::George);
    tempFamily[7].task2.completed = true;
    unsigned short curMinuteFromMidnight = timeData.startOfDay;
    uint8_t didMeeting = 0;

    for (int minute = 0; minute < day.minutesInDay; ++minute) {
        curMinuteFromMidnight++;
        for (int personLoopIdx = 0; personLoopIdx < 8; personLoopIdx++) {
            const int personIdx = familyIdxs[personLoopIdx];
            Person &person = tempFamily[personIdx]; // changed to tempFamily
            uint8_t personUint = static_cast<uint8_t>(person.name);
            Task &task1 = person.task1;
            Task &task2 = person.task2;
            // FIXME: MEETING LOGIC
            if (didMeeting < 255 && timeData.meetingStart <= curMinuteFromMidnight && curMinuteFromMidnight <= timeData.meetingEnd + 1) {
                if (curMinuteFromMidnight == timeData.meetingEnd + 1) {
                    person.curStation = person.prevStation;
                    person.prevStation = BathroomStation::None;
                    bathroom.takeStation(person.curStation, person.name);
                    didMeeting |= personUint;
                } else if (curMinuteFromMidnight == timeData.meetingStart) {
                    person.prevStation = person.curStation;
                    person.curStation = BathroomStation::None;
                    BathroomStation stationToRelease = person.curStation != BathroomStation::None ? person.curStation : BathroomStation::None;
                    bathroom.releaseStation(stationToRelease, person.name);
                }
                person.task3.completed = true;
                bathroom.occupation[personIdx] = person; // <- update occupation here too
            } else {
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
                        if (curMinuteFromMidnight >= timeData.timeDadHeatherNickLeave && personUint & dadHeatherNickMask) break;
                        if (curMinuteFromMidnight >= timeData.timeTillRulonGeorgeLeave && personUint & rulonGeorgeMask) break;
                        BathroomStation station = stations[stationIdx];
                        // if (station == BathroomStation::Shower) break;
                        if (curMinuteFromMidnight < timeData.timeTillShower && static_cast<uint8_t>(station) & showerTubMask) break;
                        if (curMinuteFromMidnight < timeData.timeTillShower && static_cast<uint8_t>(station) & showerTubMask) break;

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
        }
        // store bathroom state for this minute
        day.minutes[minute] = bathroom;
    }
    return day;
}

// MARK: Score Day
inline float scoreDay(const scheduleAPI::Day &day) {
    // Check the last minute for completion
    float score = 0;
    constexpr uint8_t sinkMask = static_cast<uint8_t>(BathroomStation::Sink1) | static_cast<uint8_t>(BathroomStation::Sink2);
    const Bathroom &finalState = day.minutes.back(); // last minute
    for (const auto &person : finalState.occupation) {
        score += person.task1.completed + person.task2.completed + person.task3.completed;
    }
    if (score != 24) return score; // Ensure chosen solution will be one with all tasks completed, don't add bonuses below
    unsigned short minuteIdx = 0;
    for (const auto &minute : day.minutes) {
        // score -= (minute.stations & showerUint && minuteIdx < showerCutoff) / 10;
        if (!minute.stations) {
            score += 0.05f;
        } else if (minute.stations & sinkMask == sinkMask) {
            score += 0.005f;
        }
        minuteIdx++;
    }
    return score;
}


// MARK: Gen Family
vector<array<Person, 8>> generateFamilyCombinations() {
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
        fam[7].task2.completed = true;
        // for (Person &person : fam) {
        //     person.task1.completed = !person.task1.timeRequired && person.task1.station == BathroomStation::Shower;
        //     person.task2.completed = !person.task2.timeRequired && person.task2.station == BathroomStation::Shower;
        // }

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
scheduleAPI::Day testDays(const array<Person, 8> &family) {
    scheduleAPI::Day bestDay = {};
    float bestDayScore = -1;
    array<int, 8> familyIdxs = {0, 1, 2, 3, 4, 5, 6, 7};
    array<int, 4> stationIdxs = {0, 1, 2, 3};
    unsigned long count = 0;
    

    sort(familyIdxs.begin(), familyIdxs.end());
    sort(stationIdxs.begin(), stationIdxs.end());
    do {
        do {
            auto day = simulateDay(familyIdxs, stationIdxs, family);
            auto curScore = scoreDay(day);

            if (curScore > bestDayScore) {
                bestDayScore = curScore;
                bestDay.minutes = day.minutes;
            }
            count++;
        } while (next_permutation(stationIdxs.begin(), stationIdxs.end()));
    } while (next_permutation(familyIdxs.begin(), familyIdxs.end()));

    return bestDay;
}

inline string formatBathroom(const Bathroom &bathroom) {
    string output;
    for (const Person& occupant : bathroom.occupation) {
        output += to_string(occupant.curStation) + ", ";
    }
    return output;
}

inline string formatDay(scheduleAPI::Day &day) {
    string output = "";
    output += "Dad, Mom, Heather, Nick, Rulon, George, Olivia, Thomas\n";
    for (const Bathroom &bathroom : day.minutes) {
        output += formatBathroom(bathroom) + "\n";
    }
    return output;
}

mutex mtx;
scheduleAPI::Day correctDay;
float correctDayScore;
atomic<bool> found = false;

// MARK: Worker
void worker(const array<Person, 8> &family, const int &ID) {
    array<Person, 8> familyCopy = family;  // local copy for thread safety
    scheduleAPI::Day day = testDays(familyCopy);
    int score = scoreDay(day);
    lock_guard<mutex> guard(mtx);
    if (score > correctDayScore) {
        correctDay.minutes = day.minutes;  // save the solution
        correctDayScore = score;
        found = true;      // signal other threads
    }
    cout << "\nWorker " << ID << " finished!\n";

    // if (!found.load()) { 
        
    // }
} // Unlock guard


// MARK: Main
int main() {
    auto startTime = chrono::high_resolution_clock::now();
    timeData.startOfDay = timeData.timeTillEnd - exampleDayGLOBAL.minutesInDay;
    Bathroom bathroom;
    scheduleAPI::Day exampleDay;

    const size_t numThreads = thread::hardware_concurrency(); // or any number you want
    ThreadPool pool(numThreads);

    scheduleAPI::Week schedule = {};
    auto combos = generateFamilyCombinations();
    cout << "Generated " << combos.size() << " family combinations.\n";
    vector<thread> threads;
    for (size_t i = 0; i < combos.size(); ++i) {
        pool.enqueue([fam = combos[i], i]() mutable {
            worker(fam, static_cast<int>(i));
        });
    }
    pool.wait();
    ofstream outputFile("save.csv");
    if (outputFile.is_open()) {
        outputFile << scoreDay(correctDay) << "\n";
        outputFile << formatDay(correctDay) << "\n";
        cout << "Score: " << scoreDay(correctDay) << "\n";
        cout << "Saved file!\n";
    }
    cout << correctDayScore << "\n";
    const Bathroom &finalState = correctDay.minutes.back(); // last minute
    for (const auto& person : finalState.occupation) {
        if (!person.task1.completed) {
            cout << to_string(person.name) << " did not complete " << to_string(person.task1.station) << "\n";
        }
        if (!person.task2.completed) {
            cout << to_string(person.name) << " did not complete " << to_string(person.task2.station) << "\n";
        }
        if (!person.task3.completed) { // task3 is meeting
            cout << to_string(person.name) << " did not complete Meeting\n";
        }
    }
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = endTime - startTime;
    auto seconds = chrono::duration_cast<chrono::milliseconds>(duration).count();
    cout << "Took " << seconds / 1000 << " seconds.\n";
    // for (int i = 0; i < sizeof())
    return 0;
}

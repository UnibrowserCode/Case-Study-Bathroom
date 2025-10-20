#pragma once
#include <type_traits>
#include "common_includes.h"
#include "global_enums.h"

namespace personAPI {

struct Task {
    bathroomAPI::BathroomStation station;
    int timeRequired;
    bool completed;
};


struct Person {
    PersonName name;
    bathroomAPI::BathroomStation curStation;
    int timeLeftUsing;
    Task task1;
    Task task2;
    Task task3;
    bathroomAPI::BathroomStation prevStation;
    int prevTimeLeftUsing = 0;
    int timeTillLeave = 460;
    inline bool allTasksCompleted() const {
        return task1.completed && task2.completed && task3.completed;
    }
    inline bool hasTimeLeftUsing() const {
        return timeLeftUsing > 0;
    }
    inline bool usingStation() const { 
        return curStation != bathroomAPI::BathroomStation::None; 
    }
    inline void resumePrevStation() {
        curStation = prevStation;
        prevStation = bathroomAPI::BathroomStation::None;
    }
    inline void takeStation(bathroomAPI::BathroomStation &station) {
        curStation = station;
    }
    inline void releaseStation() {
        curStation = bathroomAPI::BathroomStation::None;
    }
    inline void switchTask(Task &nextTask) {
        assert(prevStation == bathroomAPI::BathroomStation::None);
        prevStation = curStation;
        curStation = nextTask.station;
        prevTimeLeftUsing = timeLeftUsing;
        timeLeftUsing = nextTask.timeRequired;
    }
    inline void resumeOriginalTask() {
        curStation = prevStation;
        prevStation = bathroomAPI::BathroomStation::None;
        timeLeftUsing = prevTimeLeftUsing;
        prevTimeLeftUsing = 0;
    }
    inline void startTask(Task &task) {
        curStation = task.station;
        timeLeftUsing = task.timeRequired;
    }
};

inline std::string to_string(PersonName name) {
    switch (name) {
        case PersonName::None:    return "None";
        case PersonName::Mom:     return "Mom";
        case PersonName::Dad:     return "Dad";
        case PersonName::Heather: return "Heather";
        case PersonName::Nick:    return "Nick";
        case PersonName::Rulon:   return "Rulon";
        case PersonName::George:  return "George";
        case PersonName::Olivia:  return "Olivia";
        case PersonName::Thomas:  return "Thomas";
        default:                  return "Unknown";
    }
}
} // namespace personAPI

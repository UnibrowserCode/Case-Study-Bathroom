#include <vector>
#include <string>
#include "global_data.h"

using namespace std;


enum class Person;
class Schedule;

enum class Person {None, Mom, Dad, Heather, Nick, Rulon, George, Olivia, Thomas};

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

inline bool any(BathroomStation s) {
    return static_cast<uint32_t>(s) != 0;
}

// TODO: Get a schedule class with a vector containing Minutes.

class Schedule {};



enum class BathroomStation : uint8_t {
    None   = 0,        // 00000000
    Sink1  = 1 << 0,   // 00000001  (bit 1)
    Sink2  = 1 << 1,   // 00000010  (bit 1)
    Shower = 1 << 2,   // 00000100  (bit 2)
    Tub    = 1 << 3    // 00001000  (bit 3)
};

struct Station {
    BathroomStation type;
    Person occupiedBy;
};


class Bathroom {
    public:
        uint8_t stationsRaw;            // Userless station info for quick bit masking logic
        vector<Station> stations = {};  // Station info for complex user-specific logic
        Bathroom(uint8_t stations_=0) {
            stationsRaw = stations_;
            for (uint8_t bit = 0; bit < 8; bit++) {
                uint8_t mask = 1 << bit;
                if (mask & 0x0F) {
                    stations.push_back({static_cast<BathroomStation>(mask), Person::None});
                }
            }
        }
        inline bool isFree(BathroomStation station) const {
            return !(static_cast<uint8_t>(station) & stationsRaw);
        }
        void takeStation(BathroomStation station) {
            stationsRaw |= static_cast<uint8_t>(station);
        }
        void releaseStation(BathroomStation station) {
            stationsRaw &= static_cast<uint8_t>(station);
        }
        /*
        * brief Returns whether a station is available based on a set of rules.
        * 
        * RULES:
        * - Both sinks can be in use at the same time
        * - If someone is in the shower nobody else can be in the bathroom*
        *
        * EXCEPTIONS:
        * - Mom and Dad can be in the bathroom together even while one is showering
        * - If Thomas is bathing, others can use the sinks
        */
        inline bool stationAvailable(BathroomStation station, Person user=Person::None) {
            if (!isFree(station)) {
                return false;
            }
            if (stationsRaw == 0) {
                return true;
            }

            if (isFree(station)) {
                if (!isFree(BathroomStation::Shower)) {

                }
            }
        }
};

/*
Bathroom
 └─ resources: vector<Resource>

Resource
 ├─ id: string
 └─ conflicts: vector<Resource*>

Person
 ├─ name: string
 └─ tasks: vector<Task>

Task
 ├─ resourceNeeded: Resource*
 └─ duration: int

Schedule
 └─ entries: vector<ScheduleEntry>

ScheduleEntry
 ├─ person: Person*
 ├─ task: Task*
 ├─ startTime: int
 └─ endTime: int

Scheduler
 ├─ bathroom: Bathroom
 ├─ people: vector<Person>
 └─ findBestSchedule(): Schedule
 */
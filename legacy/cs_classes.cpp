#include <vector>
#include <string>

using namespace std;


template <typename T>
void print(T data) {
    cout << data << "\n";
}

template <typename T>
void debug(T data) {
    cout << data << "\n";
}

enum class PersonName : uint8_t;
enum class BathroomStation : uint8_t;
class Schedule;

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


//| BathroomStation operator overloads

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


//| PersonName operator overloads

inline PersonName operator|(PersonName a, PersonName b) {
    return static_cast<PersonName>(
        static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

inline PersonName operator&(PersonName a, PersonName b) {
    return static_cast<PersonName>(
        static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}

inline PersonName& operator|=(PersonName& a, PersonName b) {
    return a = a | b;
}


class Schedule {};


enum class BathroomStation : uint8_t {
    None   = 0,       // 00000000          // # 0
    Sink1  = 1 << 0,  // 00000001 (bit 1)  // # 1
    Sink2  = 1 << 1,  // 00000010 (bit 1)  // # 2
    Tub    = 1 << 2,  // 00000100 (bit 3)  // # 4
    Shower = 1 << 3   // 00001000 (bit 2)  // # 8
};


class Bathroom {
    private:
        uint8_t stations;  // 00001000
        uint8_t occupants; // 00000100
    public:
        Bathroom(uint8_t stations_=0, uint8_t occupants_=0) {  // Optional params for loading a non-empty bathroom
            stations = stations_;
            occupants = occupants_;
        }
        inline bool isFree(BathroomStation station) const {
            return !(static_cast<uint8_t>(station) & stations);
        }
        void takeStation(BathroomStation station, PersonName user) {
            stations |= static_cast<uint8_t>(station);
            occupants |= static_cast<uint8_t>(user);
        }
        void releaseStation(BathroomStation station, PersonName user) {
            stations &= ~static_cast<uint8_t>(station);
            occupants &= ~static_cast<uint8_t>(user);
        }
        /*
        * Returns whether a station is available based on a set of rules.
        * 
        * RULES:
        * - Both sinks can be in use at the same time
        * - If someone is in the shower nobody else can be in the bathroom*
        *
        * EXCEPTIONS:
        * - Mom and Dad can be in the bathroom together even while one is showering
        * - If Thomas is bathing, others can use the sinks
        */
        inline bool stationAvailable(BathroomStation station, PersonName user) {
            if (!isFree(station)) {
                return false;
            }
            if (stations == 0) {
                return true;
            }
            // Mom + Dad = 3, everyone else's baseline values are above 3.
            // If only mom and/or dad are in the bathroom, everything is open.
            if ((static_cast<uint8_t>(user) & (static_cast<uint8_t>(PersonName::Mom) | static_cast<uint8_t>(PersonName::Dad))) != 0 && (occupants & ~static_cast<uint8_t>(PersonName::Mom | PersonName::Dad)) == 0) {
                return true;
            }
            return !(stations & static_cast<uint8_t>(BathroomStation::Shower)); // Shower is the cutoff value. It should be the highest value of all the bathroom stations
        }
};


/*
Bathroom
 └─ resources: vector<Resource>

Resource
 ├─ id: string
 └─ conflicts: vector<Resource*>

PersonName
 ├─ name: string
 └─ tasks: vector<Task>

Task
 ├─ resourceNeeded: Resource*
 └─ duration: int

Schedule
 └─ entries: vector<ScheduleEntry>

ScheduleEntry
 ├─ person: PersonName*
 ├─ task: Task*
 ├─ startTime: int
 └─ endTime: int

Scheduler
 ├─ bathroom: Bathroom
 ├─ people: vector<PersonName>
 └─ findBestSchedule(): Schedule
 */
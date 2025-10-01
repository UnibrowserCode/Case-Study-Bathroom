#include "lib/bathroom.h"
#include "lib/schedule.h"
#include "lib/person.h"
#include <string>

using namespace std;
using namespace bathroomAPI;

template <typename T>
void print(T data) {
    std::cout << data << "\n";
}

template <typename T>
void debug(T data) {
    std::cout << "DEBUG: " << data << "\n";
}


Bathroom planMinute(Bathroom currentBathroom) {
    // 3393 is the number of possible states a Bathroom can be in
    Person tempPerson;
    BathroomStation tempStation;
}


scheduleAPI::Day planDay(array<BathroomStation, 4> _stations, array<Person, 8> _people) {
    scheduleAPI::Day day = {};
    Bathroom curBathroom = {};
    array<Person, 8> tempPeople = _people;
    array<BathroomStation, 4> tempStations = _stations;
    Person tempPerson;
    BathroomStation tempStation;
    for (int personIdx = 0; personIdx < 7; personIdx++) {
        if (!personIdx) {
            tempPerson = tempPeople.at(0);
            tempPeople[0] = tempPeople.at(7);
            tempPeople[7] = tempPerson;
        } else {
            tempPerson = tempPeople[personIdx];
            tempPeople[personIdx] = tempPeople[personIdx - 1];
            tempPeople[personIdx - 1] = tempPerson;
        }
        // TODO: Add station shuffling
    }
    for (int a = 0; a < day.minutesInDay; a++) {
        curBathroom = planMinute(curBathroom);
        day.minutes[a] = curBathroom;
    }
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

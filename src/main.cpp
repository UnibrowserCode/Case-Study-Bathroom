#include "lib/bathroom.h"
#include "lib/schedule.h"
#include "lib/person.h"
#include <string>

using namespace std;


template <typename T>
void print(T data) {
    std::cout << data << "\n";
}

template <typename T>
void debug(T data) {
    std::cout << "DEBUG: " << data << "\n";
}


int main() {
    bathroomAPI::Bathroom bathroom;

    personAPI::PersonName mom = PersonName::Mom;
    personAPI::PersonName dad = PersonName::Dad;
    personAPI::PersonName heather = PersonName::Heather;
    personAPI::PersonName nick = PersonName::Nick;
    personAPI::PersonName rulon = PersonName::Rulon;
    personAPI::PersonName george = PersonName::George;
    personAPI::PersonName olivia = PersonName::Olivia;
    personAPI::PersonName thomas = PersonName::Thomas;

    scheduleAPI::Week schedule = {};
    scheduleAPI::Day day = {};
    for (int i = 0; i < schedule.daysInWeek; i++) {
        for (int a = 0; a < day.minutesInDay; a++) {
            day.minutes[a] = bathroomAPI::Bathroom();
        }
    }
    // for (int i = 0; i < sizeof())
    return 0;
}

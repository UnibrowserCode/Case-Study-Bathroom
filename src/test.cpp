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

int main() {
    Bathroom bathroom;

    // Create some Person objects
    Person dad{PersonName::Dad, BathroomStation::None, 0, {BathroomStation::Sink1, 5, false}, {BathroomStation::Shower, 10, false}};
    Person mom{PersonName::Mom, BathroomStation::None, 0, {BathroomStation::Sink1, 5, false}, {BathroomStation::Shower, 8, false}};

    BathroomStation sink = BathroomStation::Sink1;

    // Test availability for Dad
    if (bathroom.stationAvailable(sink, dad.name)) {
        cout << "Dad can take Sink1\n";
        bathroom.takeStation(sink, dad.name, 1);
        dad.curStation = sink;
    } else {
        cout << "Dad cannot take Sink1 (should not happen)\n";
    }

    // Test availability for Mom (should fail)
    if (bathroom.stationAvailable(sink, mom.name)) {
        cout << to_string(mom.name) << " can take Sink1 (should not happen)\n";
    } else {
        cout << to_string(mom.name) << " cannot take Sink1\n";
    }

    // Dad releases Sink1
    bathroom.releaseStation(sink, dad.name, 1);
    dad.curStation = BathroomStation::None;

    // Test availability again for Mom
    if (bathroom.stationAvailable(sink, mom.name)) {
        cout << to_string(mom.name) << " can take Sink1 after release\n";
        bathroom.takeStation(sink, mom.name, 1);
        mom.curStation = sink;
    }

    return 0;
}

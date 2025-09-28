#include "lib/global_data.h"
#include "lib/cs_classes.h"
#include <string>

using namespace bathroom_api;
using namespace std;

void runTest(const string& testName, bool result, bool expected) {
    cout << testName << ": " << (result == expected ? "PASS" : "FAIL") << endl;
}

int main() {
    Bathroom bathroom;

    Person mom = Person::Mom;
    Person dad = Person::Dad;
    Person heather = Person::Heather;
    Person nick = Person::Nick;
    Person thomas = Person::Thomas;

    // Test 1: Mom + Dad using sinks simultaneously
    bathroom = Bathroom(); // reset
    bool momSink1 = bathroom.stationAvailable(BathroomStation::Sink1, mom);
    bathroom.takeStation(BathroomStation::Sink1, mom);
    bool dadSink2 = bathroom.stationAvailable(BathroomStation::Sink2, dad);
    runTest("Test 1 - Mom + Dad in sinks", momSink1 && dadSink2, true);

    // Test 2: Mom + Heather, should block Heather if mom is in shower or bathroom
    bathroom = Bathroom();
    bathroom.takeStation(BathroomStation::Sink1, mom);
    debug(bathroom.stationAvailable(BathroomStation::Sink2, heather));
    bool heatherSink2 = bathroom.stationAvailable(BathroomStation::Sink2, heather);
    debug(static_cast<int>(bathroom.stations));
    runTest("Test 2 - Mom + Heather blocked", heatherSink2, false);

    // Test 3: Dad + Nick, same logic
    bathroom = Bathroom();
    bathroom.takeStation(BathroomStation::Shower, dad);
    bool nickSink1 = bathroom.stationAvailable(BathroomStation::Sink1, nick);
    runTest("Test 3 - Dad + Nick blocked", nickSink1, false);

    // Test 4: Sink blocked when shower in use
    bathroom = Bathroom();
    bathroom.takeStation(BathroomStation::Shower, heather);
    bool sinkAvailable = bathroom.stationAvailable(BathroomStation::Sink1, nick);
    runTest("Test 4 - Sink blocked when shower in use", sinkAvailable, false);

    // Test 5: Sink available when bathroom empty
    bathroom = Bathroom();
    bool sinkEmpty = bathroom.stationAvailable(BathroomStation::Sink1, nick);
    runTest("Test 5 - Sink available when empty", sinkEmpty, true);

    // Test 6: Shower available when bathroom empty
    bathroom = Bathroom();
    bool showerEmpty = bathroom.stationAvailable(BathroomStation::Shower, heather);
    runTest("Test 6 - Shower available when empty", showerEmpty, true);

    // Test 7: Thomas uses tub, others can use sinks
    bathroom = Bathroom();
    bathroom.takeStation(BathroomStation::Tub, thomas);
    bool sinkAvailableWithThomas = bathroom.stationAvailable(BathroomStation::Sink1, nick);
    runTest("Test 7 - Thomas in tub, others can use sinks", sinkAvailableWithThomas, true);

    // Test 8: Thomas in tub, shower blocks others
    bathroom = Bathroom();
    bathroom.takeStation(BathroomStation::Tub, thomas);
    bathroom.takeStation(BathroomStation::Shower, mom); // mom + shower
    bool sinkBlockedWithThomasAndShower = bathroom.stationAvailable(BathroomStation::Sink1, nick);
    runTest("Test 8 - Thomas in tub, shower blocks others", sinkBlockedWithThomasAndShower, false);

    return 0;
}
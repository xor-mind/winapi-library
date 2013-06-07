#include "TestTracker.h"

int TestTracker::totalTests  = 0;
int TestTracker::currentTest = 1;
bool TestTracker::active = true;
TestTracker::Style TestTracker::defaultStyle = TestTracker::minimal;

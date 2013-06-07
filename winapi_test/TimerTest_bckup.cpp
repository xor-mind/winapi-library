#define BOOST_TEST_MODULE TimerTest
#include <boost\test\included\unit_test.hpp>
#include "winapi_library.h"

BOOST_AUTO_TEST_CASE( FixedTimeStepTest )
{
	HighPerformanceTimer timer;
	
	timer.InitTimer(4); // Set timer to 4 logic frames per second (lfps).
	
	double counter = 0.0; // keeps track of lfps.

	int startTime = GetTickCount(); // use GetTickCount to set up a one second code block, which we will use to verifiy the high performance timer.

	timer.UpdateTime0();
	
	while ( (GetTickCount() - startTime) < 1000 )
	{
		timer.UpdateTime1();

		if ((double)timer.DeltaTime() >= timer.m_LogicFrameTime)
		{
			counter += 1.0;
			timer.m_Time0 += (long long)timer.m_LogicFrameTime;
		}
	}

	counter += timer.DeltaTime()/timer.m_LogicFrameTime;
	// test within 0.25
	//BOOST_TEST_MESSAGE(counter);
	BOOST_REQUIRE_CLOSE(4, counter, 1.5); // allow timer to have a 15ms (1.5%) tolerance
}



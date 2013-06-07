#define BOOST_TEST_NO_LIB
#include <boost/test/unit_test.hpp>

#include "winapi_library.h"
#include "Prerequisites.h"
#include <iostream>

BOOST_AUTO_TEST_CASE(FixedTimeStepTest)
{
	HighPerformanceTimer hpt;
	
	const uint targetLFPS = 15; // let targetLFPS  := logic frames per a second
	double actualLFPS = 0.0;   // let actualLFPS := actual logic frames per a second
	
	hpt.InitTimer(targetLFPS);

	uint startTime = GetTickCount();

	hpt.UpdateTime0();

	while ( GetTickCount() - startTime < 1000 )
	{
		hpt.UpdateTime1();

		if (hpt.DeltaTime() >= hpt.m_LogicFrameTime)
		{
			actualLFPS += 1.0;
			hpt.m_Time0 += (long long)hpt.m_LogicFrameTime;
		}
	}

	actualLFPS += hpt.DeltaTime()/hpt.m_LogicFrameTime;
	std::cout << actualLFPS << std::endl;
	BOOST_REQUIRE_CLOSE(actualLFPS, targetLFPS, 1.5); // 1.5% = 0.015; 0.015*1000ms = 15ms;
}


#define BOOST_TEST_NO_LIB
#include <boost/test/unit_test.hpp>
#include "winapi_library.h"
#include "Prerequisites.h"
#include <iostream>

BOOST_AUTO_TEST_CASE(WriteFileTest)
{
	MemoryMap mm(_T("nreturn.txt"), MemoryMap::FileMap, MemoryMap::Read | MemoryMap::Write);
	BOOST_REQUIRE(true);
}


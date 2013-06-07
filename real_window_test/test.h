#ifndef TEST_H
#define TEST_H

#define BOOST_TEST_NO_LIB
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(adf)
{
	BOOST_REQUIRE(false);
}

#endif
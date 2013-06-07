#define BOOST_TEST_NO_LIB
#include <boost/test/unit_test.hpp>
#include "window_tests.h"

void ValidateHWND(Window* w) 
{
	BOOST_REQUIRE(w); BOOST_REQUIRE(w->Wnd());
}

int dyTwoWindows(Window* top, Window* bottom) 
{
	return bottom->yPos() - top->y1Pos();
}

float round(float f) {
    return (f > 0.0f) ? floor(f + 0.5f) : ceil(f - 0.5f);
}

void Line (HDC dc, int x1, int y1, int x2, int y2)
{
    MoveToEx (dc, x1, y1, 0);
    LineTo (dc, x2, y2);
}

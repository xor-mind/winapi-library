#include "global_operators.h"

bool operator == (const RECT& left, const RECT& right) 
{
    return ( left.left   == right.left  &&
		     left.top    == right.top   &&
			 left.right  == right.right &&
			 left.bottom == right.bottom );
};

std::ostream& operator<<(std::ostream& os, const RECT& r)
{
	os << "left: " << r.left << ", top: " << r.top << std::endl;
	os << "right: " << r.right << ", bottom: " << r.bottom << std::endl;
	return os;
}


bool operator == (const POINT& left, const POINT& right)
{
	return ( left.x == right.x && left.y == right.y);
}

std::ostream& operator<<(std::ostream& os, const POINT& p)
{
	os << "x: " << p.x << ", y: " << p.y << std::endl;
	return os;
}